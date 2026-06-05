/****************************************************************************
 * src/pwm_driver.c
 *
 * PWM Driver Implementation
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/timers/pwm.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>
#include <string.h>

#include "pwm_driver.h"
#include "param/param.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/**
 * Clamp pulse width to valid range
 *
 * @param pulse_us Input pulse width
 * @return         Clamped pulse width
 */

static inline uint32_t pwm_clamp_pulse(uint32_t pulse_us, uint8_t channel_idx)
{
  uint32_t  min_pulse   = PARAM_GET_U32(PARAM_SERVO1_MIN + channel_idx * 2);
  uint32_t  max_pulse   = PARAM_GET_U32(PARAM_SERVO1_MIN + channel_idx * 2 + 1);

  if (pulse_us < min_pulse)
    {
      pwmwarn("WARNING: Pulse %lu µs below min %lu µs, clamping\n",
              (unsigned long)pulse_us, (unsigned long)min_pulse);
      return min_pulse;
    }
  else if (pulse_us > max_pulse)
    {
      pwmwarn("WARNING: Pulse %lu µs above max %lu µs, clamping\n",
              (unsigned long)pulse_us, (unsigned long)max_pulse);
      return max_pulse;
    }
  return pulse_us;
}

/**
 * Convert pulse width (microseconds) to duty cycle (ub16 format)
 *
 * NuttX PWM uses ub16 format:
 *   - 0x0000 = 0% duty cycle
 *   - 0xFFFF = 100% duty cycle
 *   - 0x8000 = 50% duty cycle
 *
 * Formula: duty = (pulse_us / period_us) * 65536
 */

static ub16_t pulse_to_duty_ub16(uint32_t pulse_us, uint32_t period_us)
{
  if (period_us == 0)
    {
      pwmerr("ERROR: period_us is zero\n");
      return 0;
    }

  /* Calculate duty cycle: (pulse / period) * 65536 */

  uint64_t duty = ((uint64_t)pulse_us << 16) / period_us;

  /* Clamp to valid range */

  if (duty > 0xFFFF)
    {
      pwmwarn("WARNING: Duty cycle %llu exceeds max, clamping\n", duty);
      duty = 0xFFFF;
    }

  return (ub16_t)duty;
}

/**
 * Apply PWM settings to hardware
 *
 * This function:
 * 1. Sets up PWM info structure
 * 2. Calls IOCTL to set characteristics
 * 3. Calls IOCTL to start PWM
 */

static int pwm_apply_channel(struct pwm_channel_s *channel,
                             uint32_t frequency_hz, uint8_t channel_idx)
{
  struct pwm_info_s pwm_info;
  int               ret;

  if (!channel || !channel->is_open || channel->fd < 0)
    {
      pwmerr("ERROR: Invalid channel or not initialized\n");
      return -EBADF;
    }

  if (frequency_hz == 0)
    {
      pwmerr("ERROR: Invalid frequency: %lu Hz\n",
             (unsigned long)frequency_hz);
      return -EINVAL;
    }

  /* Calculate period in microseconds */

  uint32_t period_us = 1000000UL / frequency_hz;

  /* Clamp pulse width to valid range */

  uint32_t pulse_us = pwm_clamp_pulse(channel->pulse_us, channel_idx);

  /* Setup PWM info structure */

  memset(&pwm_info, 0, sizeof(struct pwm_info_s));
  pwm_info.frequency            = frequency_hz;
  pwm_info.channels[0].channel  = channel->channel_num;
  pwm_info.channels[0].duty     = pulse_to_duty_ub16(pulse_us, period_us);

  /* Set characteristics */

  ret =
    ioctl(channel->fd, PWMIOC_SETCHARACTERISTICS,
          (unsigned long)((uintptr_t)&pwm_info));
  if (ret < 0)
    {
      pwmerr("ERROR: Failed to set PWM characteristics for channel %u\n",
             channel->channel_num);
      return -errno;
    }

  /* Start PWM */

  ret = ioctl(channel->fd, PWMIOC_START, 0);
  if (ret < 0)
    {
      pwmerr("ERROR: Failed to start PWM for channel %u\n",
             channel->channel_num);
      return -errno;
    }

  return OK;
}

/*
 *  Initialize PWM Driver
 */

int pwm_driver_init(struct pwm_driver_s *driver, uint32_t frequency_hz)
{
  if (driver == NULL)
    {
      pwmerr("ERROR: Driver pointer is NULL\n");
      return -EINVAL;
    }

  if (driver->is_initialized)
    {
      return OK;
    }

  if (frequency_hz < 50 || frequency_hz > 400)
    {
      pwmerr("ERROR: Invalid frequency: %lu Hz (valid: 50-400)\n",
             (unsigned long)frequency_hz);
      return -EINVAL;
    }

  memset(driver, 0, sizeof(struct pwm_driver_s));
  driver->frequency_hz      = frequency_hz;
  driver->is_initialized    = true;
  driver->num_channels      = 0;

  /* Initialize all channel file descriptors to invalid */
  for (int i = 0; i < PARAM_GET_U32(PARAM_SERVO_NUM); i++)
    {
      driver->channels[i].fd = -1;
    }

  pwminfo("PWM driver initialized with frequency: %lu Hz\n",
          (unsigned long)frequency_hz);

  return OK;
}

/*
 *  Add PWM channel to driver
 */

int pwm_driver_add_channel(struct pwm_driver_s *driver, const char *devpath,
                           uint8_t channel_num, uint32_t initial_pulse_us)
{
  int   idx;
  int   ret;

  /* Validate driver */

  if (driver == NULL || !driver->is_initialized)
    {
      pwmerr("ERROR: Controller not initialized\n");
      return -EINVAL;
    }

  if (devpath == NULL)
    {
      pwmerr("ERROR: Device path is NULL\n");
      return -EINVAL;
    }

  /* Check if we have space */

  if (driver->num_channels >= PARAM_GET_U32(PARAM_SERVO_NUM))
    {
      pwmerr("ERROR: Maximum number of channels reached\n");
      return -ENOMEM;
    }

  /* Open PWM device */

  int fd = open(devpath, O_WRONLY);

  if (fd < 0)
    {
      int errcode = errno;
      pwmerr("ERROR: Failed to open %s: %d\n", devpath, errcode);
      return -errcode;
    }

  idx = driver->num_channels;

  driver->channels[idx].fd          = fd;
  driver->channels[idx].channel_num = channel_num;
  driver->channels[idx].pulse_us    = initial_pulse_us;
  driver->channels[idx].is_open     = true;

  /* Apply initial settings to hardware */

  ret = pwm_apply_channel(&driver->channels[idx], driver->frequency_hz, idx);
  if (ret < 0)
    {
      pwmerr("ERROR: Failed to apply initial settings: %d\n", ret);
      close(fd);
      driver->channels[idx].fd      = -1;
      driver->channels[idx].is_open = false;
      return ret;
    }

  driver->num_channels++;

  pwminfo("Added channel %d: %s (ch%d) @ %lu µs\n", idx, devpath, channel_num,
          (unsigned long)driver->channels[idx].pulse_us);

  return OK;
}

/**
 * Set pulse width for one channel
 */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int pwm_driver_set_pulse(struct pwm_driver_s *driver, uint8_t channel_idx,
                         uint32_t pulse_us)
{
  struct pwm_channel_s *channel;

  /* Validate driver */
  if (driver == NULL || !driver->is_initialized)
    {
      pwmerr("ERROR: Controller not initialized\n");
      return -EINVAL;
    }

  /* Validate channel index */
  if (channel_idx >= driver->num_channels)
    {
      pwmerr("ERROR: Invalid channel index: %u\n", channel_idx);
      return -EINVAL;
    }

  channel = &driver->channels[channel_idx];

  if (channel == NULL || !channel->is_open)
    {
      pwmerr("ERROR: Channel %u not initialized\n", channel_idx);
      return -EINVAL;
    }

  channel->pulse_us = pulse_us;

  return pwm_apply_channel(channel, driver->frequency_hz, channel_idx);
}

/**
 * Set pulse widths for all channels
 */

int pwm_driver_set_all_pulses(struct pwm_driver_s *driver,
                              const uint32_t *pulse_us_array,
                              uint8_t num_pulses)
{
  int ret = OK;

  /* Validate driver */
  if (driver == NULL || !driver->is_initialized)
    {
      pwmerr("ERROR: Driver not initialized\n");
      return -EINVAL;
    }

  if (pulse_us_array == NULL)
    {
      pwmerr("ERROR: pulse_us_array is NULL\n");
      return -EINVAL;
    }

  if (num_pulses > driver->num_channels)
    {
      pwmwarn("WARNING: num_pulses (%d) > num_channels (%d), limiting\n",
              num_pulses, driver->num_channels);
      num_pulses = driver->num_channels;
    }

  /* Set pulse widths for each channel */
  for (int i = 0; i < num_pulses; i++)
    {
      ret = pwm_driver_set_pulse(driver, i, pulse_us_array[i]);
      if (ret < 0)
        {
          pwmerr("ERROR: Failed to set channel %d: %d\n", i, ret);

          /* Continue processing other channels but return error */
        }
    }

  return ret;
}

/**
 * Stop all PWM outputs
 */

void pwm_driver_stop_all(struct pwm_driver_s *driver)
{
  /* Validate driver */

  if (driver == NULL || !driver->is_initialized)
    {
      pwmerr("ERROR: Driver not initialized\n");
      return;
    }

  pwminfo("Stopping all PWM outputs\n");

  /* Stop each channel */

  for (int i = 0; i < driver->num_channels; i++)
    {
      if (driver->channels[i].is_open && driver->channels[i].fd >= 0)
        {
          ioctl(driver->channels[i].fd, PWMIOC_STOP, 0);
        }
    }
}

/**
 * Cleanup PWM controller
 */

void pwm_driver_deinit(struct pwm_driver_s *driver)
{
  if (driver == NULL || !driver->is_initialized)
    {
      pwmerr("ERROR: Driver not initialized\n");
      return;
    }

  pwminfo("Cleaning up PWM driver\n");

  /* Stop and close all channels */

  for (int i = 0; i < driver->num_channels; i++)
    {
      if (driver->channels[i].is_open)
        {
          /* Stop PWM */

          if (driver->channels[i].fd >= 0)
            {
              ioctl(driver->channels[i].fd, PWMIOC_STOP, 0);
              close(driver->channels[i].fd);
            }

          /* Clear channel state */

          driver->channels[i].fd        = -1;
          driver->channels[i].is_open   = false;
        }
    }

  /* Clear driver state */

  driver->num_channels      = 0;
  driver->is_initialized    = false;

  pwminfo("PWM driver deinitialized\n");
}
