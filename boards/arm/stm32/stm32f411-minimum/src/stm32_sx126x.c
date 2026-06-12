/****************************************************************************
 * boards/arm/stm32/stm32f411-minimum/src/stm32_sx126x.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <debug.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>

#include <nuttx/sched.h>
#include <nuttx/spi/spi.h>
#include <nuttx/wireless/lpwan/sx126x.h>

#include "stm32.h"
#include "stm32_exti.h"
#include "stm32_gpio.h"
#include "stm32_spi.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_STM32F411MINIMUM_SX126X_SPI_PORT
#  define CONFIG_STM32F411MINIMUM_SX126X_SPI_PORT 2
#endif

#ifndef CONFIG_STM32F411MINIMUM_SX126X_DEVPATH
#  define CONFIG_STM32F411MINIMUM_SX126X_DEVPATH "/dev/sx126x0"
#endif

#ifndef CONFIG_STM32F411MINIMUM_SX126X_FREQ_MIN
#  define CONFIG_STM32F411MINIMUM_SX126X_FREQ_MIN 860000000
#endif

#ifndef CONFIG_STM32F411MINIMUM_SX126X_FREQ_MAX
#  define CONFIG_STM32F411MINIMUM_SX126X_FREQ_MAX 930000000
#endif

#ifndef CONFIG_STM32F411MINIMUM_SX126X_TXPOWER_MAX
#  define CONFIG_STM32F411MINIMUM_SX126X_TXPOWER_MAX 22
#endif

#ifndef GPIO_SX126X_RESET
#  define GPIO_SX126X_RESET (GPIO_PORTB|GPIO_PIN0|GPIO_OUTPUT_SET|GPIO_OUTPUT|GPIO_PUSHPULL|GPIO_SPEED_2MHz)
#endif

#ifndef GPIO_SX126X_DIO1
#  define GPIO_SX126X_DIO1 (GPIO_PORTB|GPIO_PIN1|GPIO_INPUT|GPIO_FLOAT|GPIO_EXTI)
#endif

#define SX126X_IRQ_DIO1_MASK (SX126X_IRQ_TXDONE_MASK|SX126X_IRQ_RXDONE_MASK|SX126X_IRQ_CRCERR_MASK|SX126X_IRQ_CADDONE_MASK| \
   SX126X_IRQ_CADDETECTED_MASK | SX126X_IRQ_TIMEOUT_MASK)

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void sx126x_board_reset(void);
static int sx126x_board_irq0attach(xcpt_t handler, FAR void *arg);
static int sx126x_board_get_pa_values(FAR enum sx126x_device_e *model,
                                      FAR uint8_t *hpmax,
                                      FAR uint8_t *padutycycle);
static int sx126x_board_limit_tx_power(FAR uint8_t *current_power);
static int sx126x_board_check_frequency(uint32_t frequency);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct sx126x_lower_s g_sx126x_lower =
{
  .dev_number        = 0,
  .reset             = sx126x_board_reset,
  .masks             =
    {
      .dio1_mask = SX126X_IRQ_DIO1_MASK,
      .dio2_mask = 0,
      .dio3_mask = 0,
    },
  .dio3_voltage      = SX126X_TCXO_1_8V,
  .dio3_delay        = 0,
  .use_dio2_as_rf_sw = 1,
  .irq0attach        = sx126x_board_irq0attach,
  .regulator_mode    = SX126X_DC_DC_LDO,
  .get_pa_values     = sx126x_board_get_pa_values,
  .limit_tx_power    = sx126x_board_limit_tx_power,
  .tx_ramp_time      = SX126X_SET_RAMP_200U,
  .check_frequency   = sx126x_board_check_frequency,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sx126x_board_reset
 ****************************************************************************/

static void sx126x_board_reset(void)
{
  stm32_configgpio(GPIO_SX126X_RESET);
  stm32_gpiowrite(GPIO_SX126X_RESET, false);
  nxsched_usleep(1000);

  stm32_gpiowrite(GPIO_SX126X_RESET, true);
  nxsched_usleep(10000);
}

/****************************************************************************
 * Name: sx126x_board_irq0attach
 ****************************************************************************/

static int sx126x_board_irq0attach(xcpt_t handler, FAR void *arg)
{
  return stm32_gpiosetevent(GPIO_SX126X_DIO1, true, false, false, handler, arg);
}

/****************************************************************************
 * Name: sx126x_board_get_pa_values
 ****************************************************************************/

static int sx126x_board_get_pa_values(FAR enum sx126x_device_e *model,
                                      FAR uint8_t *hpmax,
                                      FAR uint8_t *padutycycle)
{
  if (model == NULL || hpmax == NULL || padutycycle == NULL)
    {
      return -EINVAL;
    }

  *model = SX1262;
  *hpmax = 0x07;
  *padutycycle = 0x04;

  return OK;
}

/****************************************************************************
 * Name: sx126x_board_limit_tx_power
 ****************************************************************************/

static int sx126x_board_limit_tx_power(FAR uint8_t *current_power)
{
  if (current_power == NULL)
    {
      return -EINVAL;
    }

  if (*current_power > CONFIG_STM32F411MINIMUM_SX126X_TXPOWER_MAX)
    {
      *current_power = CONFIG_STM32F411MINIMUM_SX126X_TXPOWER_MAX;
    }

  return OK;
}

/****************************************************************************
 * Name: sx126x_board_check_frequency
 ****************************************************************************/

static int sx126x_board_check_frequency(uint32_t frequency)
{
  if (frequency < CONFIG_STM32F411MINIMUM_SX126X_FREQ_MIN ||
      frequency > CONFIG_STM32F411MINIMUM_SX126X_FREQ_MAX)
    {
      wlerr("sx126x: unsupported frequency %" PRIu32 "\n", frequency);
      return -EINVAL;
    }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_sx126x_initialize
 *
 * Description:
 *   Initialize SX126x LoRa transceiver and register the character device.
 *
 ****************************************************************************/

int stm32_sx126x_initialize(void)
{
  FAR struct spi_dev_s *spi;

  stm32_configgpio(GPIO_SX126X_DIO1);

  spi = stm32_spibus_initialize(CONFIG_STM32F411MINIMUM_SX126X_SPI_PORT);
  if (spi == NULL)
    {
      wlerr("sx126x: failed to initialize SPI%d\n",
            CONFIG_STM32F411MINIMUM_SX126X_SPI_PORT);
      return -ENODEV;
    }

  sx126x_register(spi, &g_sx126x_lower,
                  CONFIG_STM32F411MINIMUM_SX126X_DEVPATH);

  return OK;
}
