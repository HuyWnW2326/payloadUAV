/****************************************************************************
 * boards/arm/stm32/stm32f411-minimum/src/stm32_pwm.c
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

#include <sys/types.h>
#include <errno.h>
#include <nuttx/debug.h>

#include <nuttx/board.h>
#include <nuttx/timers/pwm.h>

#include <arch/board/board.h>

#include "chip.h"
#include "arm_internal.h"
#include "stm32_pwm.h"
#include "stm32f411-minimum.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* PWM
 *
 * The stm32f411-minimum has no real on-board PWM devices, but the board can
 * be configured to output a pulse train using TIM4 CH2.
 * This pin is used by FSMC is connect to CN5 just for this purpose:
 *
 * PB0 ADC12_IN8/TIM3_CH3
 *
 */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_pwm_setup
 *
 * Description:
 *   Initialize PWM and register the PWM device.
 *
 ****************************************************************************/

int stm32_pwm_setup(void)
{
    static bool initialized = false;
    struct pwm_lowerhalf_s *pwm;
    int ret;

    if (initialized) return OK;

    /* TIM2 → /dev/pwm0 (CH1, CH2 — Servo 1, 2) */
#ifdef CONFIG_STM32_TIM2_PWM
    pwm = stm32_pwminitialize(2);
    if (!pwm)
    {
        aerr("ERROR: Failed to get TIM2 PWM\n");
        return -ENODEV;
    }
    ret = pwm_register("/dev/pwm0", pwm);
    if (ret < 0)
    {
        aerr("ERROR: pwm_register /dev/pwm0 failed: %d\n", ret);
        return ret;
    }
#endif

    /* TIM3 → /dev/pwm1 (CH1, CH2, CH3, CH4 — Servo 3, 4, 5, 6) */
#ifdef CONFIG_STM32_TIM3_PWM
    pwm = stm32_pwminitialize(3);
    if (!pwm)
    {
        aerr("ERROR: Failed to get TIM3 PWM\n");
        return -ENODEV;
    }
    ret = pwm_register("/dev/pwm1", pwm);
    if (ret < 0)
    {
        aerr("ERROR: pwm_register /dev/pwm1 failed: %d\n", ret);
        return ret;
    }
#endif

    initialized = true;
    return OK;
}
