/****************************************************************************
 * common/sbus/sbus_driver.h
 *
 * SBUS driver interface.
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
#include <semaphore.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define SBUS_FRAME_SIZE      25
#define SBUS_BUFFER_SIZE     (SBUS_FRAME_SIZE + SBUS_FRAME_SIZE / 2)
#define SBUS_MAX_CHANNELS    16
#define SBUS_BAUDRATE        100000
#define SBUS_START_BYTE      0x0f
#define SBUS_FRAME_GAP_US    4000

#define SBUS_FLAG_CH17       (1 << 0)
#define SBUS_FLAG_CH18       (1 << 1)
#define SBUS_FLAG_FRAME_LOST (1 << 2)
#define SBUS_FLAG_FAILSAFE   (1 << 3)

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct sbus_driver_s;
struct sbus_channels_s;

typedef void (*sbus_channels_callback_t)(struct sbus_driver_s *driver,
                                         const struct sbus_channels_s *channels);

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
  uint8_t frame_buf[SBUS_BUFFER_SIZE];
  uint8_t frame_len;
  uint64_t last_rx_time_us;
  uint32_t frame_drops;
  sem_t lock;
  struct sbus_channels_s latest_channels;
  sbus_channels_callback_t channels_callback;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * Initialize SBUS driver.
 *
 * @param driver   Pointer to driver structure
 * @param devpath  Path to UART device (e.g., "/dev/ttyS1")
 * @return         OK on success, negative errno on failure
 */

int sbus_driver_init(struct sbus_driver_s *driver, const char *devpath);

/**
 * Read one byte from SBUS UART.
 *
 * @param driver  Pointer to driver structure
 * @param byte    Pointer to byte output
 * @return        OK on success, negative errno on failure
 */

int sbus_driver_read_byte(struct sbus_driver_s *driver, uint8_t *byte);

/**
 * Read one raw SBUS frame.
 *
 * @param driver  Pointer to driver structure
 * @param frame   Pointer to frame output
 * @return        OK on success, negative errno on failure
 */

int sbus_driver_read_frame(struct sbus_driver_s *driver,
                           struct sbus_frame_s *frame);

/**
 * Parse one raw SBUS frame into channel values.
 *
 * @param frame     Pointer to raw SBUS frame
 * @param channels  Pointer to decoded channel output
 * @return          OK on success, negative errno on failure
 */

int sbus_driver_parse_frame(const struct sbus_frame_s *frame,
                            struct sbus_channels_s *channels);

/**
 * Read and parse one SBUS frame.
 *
 * @param driver    Pointer to driver structure
 * @param channels  Pointer to decoded channel output
 * @return          OK on success, negative errno on failure
 */

int sbus_driver_read_channels(struct sbus_driver_s *driver,
                              struct sbus_channels_s *channels);

/**
 * Set optional callback called from the SBUS run loop after each valid frame.
 *
 * @param driver    Pointer to driver structure
 * @param callback  Callback function, or NULL to disable
 * @return          OK on success, negative errno on failure
 */

int sbus_driver_set_callback(struct sbus_driver_s *driver,
                             sbus_channels_callback_t callback);

/**
 * Run SBUS receive loop.
 *
 * This function is intended to be called from a dedicated task/thread.
 *
 * @param driver  Pointer to driver structure
 * @return        OK on clean stop, negative errno on failure
 */

int sbus_driver_run(struct sbus_driver_s *driver);

/**
 * Stop SBUS receive loop.
 *
 * @param driver  Pointer to driver structure
 */

void sbus_driver_stop(struct sbus_driver_s *driver);

/**
 * Copy latest decoded SBUS channels.
 *
 * @param driver    Pointer to driver structure
 * @param channels  Pointer to decoded channel output
 * @return          OK on success, -EAGAIN if no frame has been received
 */

int sbus_driver_get_latest(struct sbus_driver_s *driver,
                           struct sbus_channels_s *channels);

/**
 * Get number of dropped/invalid frames detected by the parser.
 *
 * @param driver  Pointer to driver structure
 * @return        Drop count, or 0 if driver is NULL
 */

uint32_t sbus_driver_dropped_frames(struct sbus_driver_s *driver);

/**
 * Cleanup SBUS driver.
 *
 * @param driver  Pointer to driver structure
 */

void sbus_driver_deinit(struct sbus_driver_s *driver);

#endif /* __COMMON_SBUS_DRIVER_H */
