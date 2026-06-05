/****************************************************************************
 * common/pwm/pwm_driver.h
 *
 * PWM driver interface.
 *
 ****************************************************************************/

#ifndef __INDUSTRY_PAYLOAD_CONTROLLER_PWM_DRIVER_H
#define __INDUSTRY_PAYLOAD_CONTROLLER_PWM_DRIVER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>
#include <stdbool.h>

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* PWM channel state */

struct pwm_channel_s
{
  int fd;               /* File descriptor for the PWM device */
  uint8_t channel_num;  /* PWM channel number of timer */
  uint32_t pulse_us;    /* Pulse width in microseconds */
  bool is_open;         /* Channel is initialized */
};

/* PWM driver context */

struct pwm_driver_s
{
  struct pwm_channel_s channels[6]; /* Array of PWM channels */
  uint8_t num_channels;             /* Number of active channels */
  uint32_t frequency_hz;            /* PWM frequency in Hz */
  bool is_initialized;              /* Driver is initialized */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * Initialize PWM driver
 *
 * @param driver       Pointer to driver structure
 * @param frequency_hz PWM frequency in Hz (typically 50 for servos)
 * @return             OK on success, negative errno on failure
 */

int pwm_driver_init(struct pwm_driver_s *driver, uint32_t frequency_hz);

/**
 * Add PWM channel to driver
 *
 * @param driver            Pointer to driver structure
 * @param devpath           Path to PWM device (e.g., "/dev/pwm0")
 * @param channel_num       PWM channel number on the timer
 * @param initial_pulse_us  Initial pulse width in microseconds
 * @return                  OK on success, negative errno on failure
 */

int pwm_driver_add_channel(struct pwm_driver_s *driver, const char *devpath,
                           uint8_t channel_num, uint32_t initial_pulse_us);

/**
 * Set pulse width for one channel
 *
 * @param driver        Pointer to driver structure
 * @param channel_idx   Index of the channel to set (0 to num_channels-1)
 * @param pulse_us      Pulse width in microseconds
 * @return              OK on success, negative errno on failure
 */

int pwm_driver_set_pulse(struct pwm_driver_s *driver, uint8_t channel_idx,
                         uint32_t pulse_us);

/**
 * Set pulse widths for all channels
 *
 * @param driver            Pointer to driver structure
 * @param pulse_us_array    Array of pulse widths in microseconds
 * @param num_pulses        Number of pulses to set (should not exceed
 * num_channels)
 * @return                  OK on success, negative errno on failure
 */

int pwm_driver_set_all_pulses(struct pwm_driver_s *driver,
                              const uint32_t *pulse_us_array,
                              uint8_t num_pulses);

/**
 * Stop all PWM outputs
 *
 * @param driver    Pointer to driver structure
 */

void pwm_driver_stop_all(struct pwm_driver_s *driver);

/**
 * Cleanup PWM driver
 *
 * @param driver    Pointer to driver structure
 */

void pwm_driver_deinit(struct pwm_driver_s *driver);

#endif /* __INDUSTRY_PAYLOAD_CONTROLLER_PWM_DRIVER_H */
