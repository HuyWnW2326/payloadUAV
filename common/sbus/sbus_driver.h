/****************************************************************************
 * common/sbus/sbus_driver.h
 *
 * Minimal SBUS driver interface.
 *
 ****************************************************************************/

#ifndef __COMMON_SBUS_DRIVER_H
#define __COMMON_SBUS_DRIVER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>
#include <stdbool.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define SBUS_FRAME_SIZE      25
#define SBUS_MAX_CHANNELS    16
#define SBUS_BAUDRATE        100000
#define SBUS_START_BYTE      0x0f

#define SBUS_FLAG_CH17       (1 << 0)
#define SBUS_FLAG_CH18       (1 << 1)
#define SBUS_FLAG_FRAME_LOST (1 << 2)
#define SBUS_FLAG_FAILSAFE   (1 << 3)

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct sbus_frame_s
{
  uint8_t data[SBUS_FRAME_SIZE];
};

struct sbus_channels_s
{
  uint16_t channels[SBUS_MAX_CHANNELS];
  bool ch17;
  bool ch18;
  bool frame_lost;
  bool failsafe;
};

struct sbus_driver_s
{
  int fd;
  volatile bool is_running;
  bool is_initialized;
  bool has_channels;
  struct sbus_channels_s latest_channels;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

int sbus_driver_init(struct sbus_driver_s *driver, const char *devpath);

int sbus_driver_read_frame(struct sbus_driver_s *driver,
                           struct sbus_frame_s *frame);

int sbus_driver_decode_frame(const struct sbus_frame_s *frame,
                             struct sbus_channels_s *channels);

int sbus_driver_read_channels(struct sbus_driver_s *driver,
                              struct sbus_channels_s *channels);

int sbus_driver_run(struct sbus_driver_s *driver);

void sbus_driver_stop(struct sbus_driver_s *driver);

void sbus_driver_deinit(struct sbus_driver_s *driver);

#endif /* __COMMON_SBUS_DRIVER_H */
