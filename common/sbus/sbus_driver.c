/****************************************************************************
 * common/sbus/sbus_driver.c
 *
 * SBUS Driver Implementation
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <debug.h>
#include <string.h>
#include <poll.h>
#include <time.h>

#include "sbus_driver.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define SBUS_POLL_TIMEOUT_MS 100

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int sbus_config_uart(int fd)
{
  struct termios tio;

  if (tcgetattr(fd, &tio) < 0)
    {
      int errcode = errno;
      _err("ERROR: SBUS tcgetattr failed: %d\n", errcode);
      return -errcode;
    }

  cfmakeraw(&tio);
  cfsetispeed(&tio, SBUS_BAUDRATE);
  cfsetospeed(&tio, SBUS_BAUDRATE);

  tio.c_cflag &= ~CSIZE;
  tio.c_cflag |= CS8;
  tio.c_cflag |= PARENB;
  tio.c_cflag &= ~PARODD;
  tio.c_cflag |= CSTOPB;
  tio.c_cflag |= (CLOCAL | CREAD);
  tio.c_cflag &= ~CRTSCTS;

  if (tcsetattr(fd, TCSANOW, &tio) < 0)
    {
      int errcode = errno;
      _err("ERROR: SBUS tcsetattr failed: %d\n", errcode);
      return -errcode;
    }

  tcflush(fd, TCIOFLUSH);

  return OK;
}

static uint64_t sbus_time_us(void)
{
  struct timespec ts;

  if (clock_gettime(CLOCK_MONOTONIC, &ts) < 0)
    {
      return 0;
    }

  return (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
}

static bool sbus_end_byte_valid(uint8_t byte)
{
  switch (byte)
    {
      case 0x00: /* SBUS1 */
      case 0x04: /* SBUS2 receiver voltage slot */
      case 0x14: /* SBUS2 GPS/baro slot */
      case 0x24: /* SBUS2 data slot */
      case 0x34: /* SBUS2 data slot */
        return true;

      default:
        return false;
    }
}

static int sbus_lock(struct sbus_driver_s *driver)
{
  int ret;

  do
    {
      ret = sem_wait(&driver->lock);
    }
  while (ret < 0 && errno == EINTR);

  if (ret < 0)
    {
      int errcode = errno;
      _err("ERROR: SBUS sem_wait failed: %d\n", errcode);
      return -errcode;
    }

  return OK;
}

static void sbus_unlock(struct sbus_driver_s *driver)
{
  sem_post(&driver->lock);
}

static void sbus_parser_reset(struct sbus_driver_s *driver)
{
  driver->frame_len = 0;
}

static void sbus_recover_from_decode_fail(struct sbus_driver_s *driver)
{
  uint8_t start_index;

  driver->frame_drops++;

  for (start_index = 1; start_index < driver->frame_len; start_index++)
    {
      if (driver->frame_buf[start_index] == SBUS_START_BYTE)
        {
          memmove(driver->frame_buf, &driver->frame_buf[start_index],
                  driver->frame_len - start_index);
          driver->frame_len -= start_index;
          return;
        }
    }

  sbus_parser_reset(driver);
}

static int sbus_parse_byte(struct sbus_driver_s *driver, uint8_t byte,
                           struct sbus_frame_s *frame)
{
  uint64_t now_us;

  now_us = sbus_time_us();
  if (driver->last_rx_time_us != 0 &&
      now_us > driver->last_rx_time_us &&
      now_us - driver->last_rx_time_us > SBUS_FRAME_GAP_US)
    {
      sbus_parser_reset(driver);
    }

  driver->last_rx_time_us = now_us;

  if (driver->frame_len == 0)
    {
      if (byte != SBUS_START_BYTE)
        {
          return -EAGAIN;
        }

      driver->frame_buf[driver->frame_len++] = byte;
      return -EAGAIN;
    }

  if (driver->frame_len >= SBUS_BUFFER_SIZE)
    {
      sbus_recover_from_decode_fail(driver);
      return -EAGAIN;
    }

  driver->frame_buf[driver->frame_len++] = byte;

  if (driver->frame_len < SBUS_FRAME_SIZE)
    {
      return -EAGAIN;
    }

  memcpy(frame->data, driver->frame_buf, SBUS_FRAME_SIZE);

  if (frame->data[0] != SBUS_START_BYTE ||
      !sbus_end_byte_valid(frame->data[SBUS_FRAME_SIZE - 1]))
    {
      sbus_recover_from_decode_fail(driver);
      return -EAGAIN;
    }

  sbus_parser_reset(driver);

  return OK;
}

static int sbus_update_latest(struct sbus_driver_s *driver,
                              const struct sbus_channels_s *channels)
{
  int ret;

  ret = sbus_lock(driver);
  if (ret < 0)
    {
      return ret;
    }

  memcpy(&driver->latest_channels, channels,
         sizeof(struct sbus_channels_s));
  driver->has_channels = true;

  sbus_unlock(driver);

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int sbus_driver_init(struct sbus_driver_s *driver, const char *devpath)
{
  int fd;
  int ret;

  if (driver == NULL)
    {
      _err("ERROR: SBUS driver pointer is NULL\n");
      return -EINVAL;
    }

  if (devpath == NULL)
    {
      _err("ERROR: SBUS device path is NULL\n");
      return -EINVAL;
    }

  if (driver->is_initialized)
    {
      return OK;
    }

  memset(driver, 0, sizeof(struct sbus_driver_s));
  driver->fd = -1;

  ret = sem_init(&driver->lock, 0, 1);
  if (ret < 0)
    {
      int errcode = errno;
      _err("ERROR: SBUS sem_init failed: %d\n", errcode);
      return -errcode;
    }

  fd = open(devpath, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd < 0)
    {
      int errcode = errno;
      _err("ERROR: Failed to open %s: %d\n", devpath, errcode);
      sem_destroy(&driver->lock);
      return -errcode;
    }

  ret = sbus_config_uart(fd);
  if (ret < 0)
    {
      close(fd);
      sem_destroy(&driver->lock);
      return ret;
    }

  driver->fd             = fd;
  driver->is_initialized = true;
  driver->last_rx_time_us = sbus_time_us();

  _info("SBUS driver initialized: %s\n", devpath);

  return OK;
}

int sbus_driver_read_byte(struct sbus_driver_s *driver, uint8_t *byte)
{
  struct pollfd pfd;
  ssize_t nread;
  int ret;

  if (driver == NULL || !driver->is_initialized || driver->fd < 0)
    {
      _err("ERROR: SBUS driver not initialized\n");
      return -EINVAL;
    }

  if (byte == NULL)
    {
      _err("ERROR: SBUS byte pointer is NULL\n");
      return -EINVAL;
    }

  pfd.fd      = driver->fd;
  pfd.events  = POLLIN;
  pfd.revents = 0;

  ret = poll(&pfd, 1, SBUS_POLL_TIMEOUT_MS);
  if (ret < 0)
    {
      int errcode = errno;
      _err("ERROR: SBUS poll failed: %d\n", errcode);
      return -errcode;
    }

  if (ret == 0)
    {
      return -EAGAIN;
    }

  if (pfd.revents & (POLLERR | POLLHUP))
    {
      _err("ERROR: SBUS UART poll error: 0x%02lx\n",
           (unsigned long)pfd.revents);
      return -EIO;
    }

  nread = read(driver->fd, byte, 1);
  if (nread < 0)
    {
      int errcode = errno;
      if (errcode == EAGAIN || errcode == EWOULDBLOCK)
        {
          return -EAGAIN;
        }

      _err("ERROR: Failed to read SBUS byte: %d\n", errcode);
      return -errcode;
    }

  if (nread != 1)
    {
      return -EIO;
    }

  return OK;
}

int sbus_driver_read_frame(struct sbus_driver_s *driver,
                           struct sbus_frame_s *frame)
{
  uint8_t byte;
  int ret;

  if (driver == NULL || !driver->is_initialized || driver->fd < 0)
    {
      _err("ERROR: SBUS driver not initialized\n");
      return -EINVAL;
    }

  if (frame == NULL)
    {
      _err("ERROR: SBUS frame pointer is NULL\n");
      return -EINVAL;
    }

  for (; ; )
    {
      ret = sbus_driver_read_byte(driver, &byte);
      if (ret < 0)
        {
          return ret;
        }

      ret = sbus_parse_byte(driver, byte, frame);
      if (ret == OK)
        {
          return OK;
        }

      if (ret != -EAGAIN)
        {
          return ret;
        }
    }
}

int sbus_driver_parse_frame(const struct sbus_frame_s *frame,
                            struct sbus_channels_s *channels)
{
  uint8_t flags;

  if (frame == NULL)
    {
      _err("ERROR: SBUS frame pointer is NULL\n");
      return -EINVAL;
    }

  if (channels == NULL)
    {
      _err("ERROR: SBUS channels pointer is NULL\n");
      return -EINVAL;
    }

  if (frame->data[0] != SBUS_START_BYTE)
    {
      _err("ERROR: Invalid SBUS start byte: 0x%02x\n", frame->data[0]);
      return -EINVAL;
    }

  if (!sbus_end_byte_valid(frame->data[SBUS_FRAME_SIZE - 1]))
    {
      _err("ERROR: Invalid SBUS end byte: 0x%02x\n",
           frame->data[SBUS_FRAME_SIZE - 1]);
      return -EINVAL;
    }

  memset(channels, 0, sizeof(struct sbus_channels_s));

  channels->channels[0]  = ((frame->data[1] |
                            frame->data[2] << 8) & 0x07ff);
  channels->channels[1]  = ((frame->data[2] >> 3 |
                            frame->data[3] << 5) & 0x07ff);
  channels->channels[2]  = ((frame->data[3] >> 6 |
                            frame->data[4] << 2 |
                            frame->data[5] << 10) & 0x07ff);
  channels->channels[3]  = ((frame->data[5] >> 1 |
                            frame->data[6] << 7) & 0x07ff);
  channels->channels[4]  = ((frame->data[6] >> 4 |
                            frame->data[7] << 4) & 0x07ff);
  channels->channels[5]  = ((frame->data[7] >> 7 |
                            frame->data[8] << 1 |
                            frame->data[9] << 9) & 0x07ff);
  channels->channels[6]  = ((frame->data[9] >> 2 |
                            frame->data[10] << 6) & 0x07ff);
  channels->channels[7]  = ((frame->data[10] >> 5 |
                            frame->data[11] << 3) & 0x07ff);
  channels->channels[8]  = ((frame->data[12] |
                            frame->data[13] << 8) & 0x07ff);
  channels->channels[9]  = ((frame->data[13] >> 3 |
                            frame->data[14] << 5) & 0x07ff);
  channels->channels[10] = ((frame->data[14] >> 6 |
                            frame->data[15] << 2 |
                            frame->data[16] << 10) & 0x07ff);
  channels->channels[11] = ((frame->data[16] >> 1 |
                            frame->data[17] << 7) & 0x07ff);
  channels->channels[12] = ((frame->data[17] >> 4 |
                            frame->data[18] << 4) & 0x07ff);
  channels->channels[13] = ((frame->data[18] >> 7 |
                            frame->data[19] << 1 |
                            frame->data[20] << 9) & 0x07ff);
  channels->channels[14] = ((frame->data[20] >> 2 |
                            frame->data[21] << 6) & 0x07ff);
  channels->channels[15] = ((frame->data[21] >> 5 |
                            frame->data[22] << 3) & 0x07ff);

  flags = frame->data[23];
  channels->ch17       = (flags & SBUS_FLAG_CH17) != 0;
  channels->ch18       = (flags & SBUS_FLAG_CH18) != 0;
  channels->frame_lost = (flags & SBUS_FLAG_FRAME_LOST) != 0;
  channels->failsafe   = (flags & SBUS_FLAG_FAILSAFE) != 0;

  return OK;
}

int sbus_driver_read_channels(struct sbus_driver_s *driver,
                              struct sbus_channels_s *channels)
{
  struct sbus_frame_s frame;
  int ret;

  ret = sbus_driver_read_frame(driver, &frame);
  if (ret < 0)
    {
      return ret;
    }

  ret = sbus_driver_parse_frame(&frame, channels);
  if (ret < 0)
    {
      if (driver != NULL)
        {
          driver->frame_drops++;
        }

      return ret;
    }

  return sbus_update_latest(driver, channels);
}

int sbus_driver_set_callback(struct sbus_driver_s *driver,
                             sbus_channels_callback_t callback)
{
  int ret;

  if (driver == NULL || !driver->is_initialized)
    {
      _err("ERROR: SBUS driver not initialized\n");
      return -EINVAL;
    }

  ret = sbus_lock(driver);
  if (ret < 0)
    {
      return ret;
    }

  driver->channels_callback = callback;

  sbus_unlock(driver);

  return OK;
}

int sbus_driver_run(struct sbus_driver_s *driver)
{
  struct sbus_channels_s channels;
  sbus_channels_callback_t callback;
  int ret;

  if (driver == NULL || !driver->is_initialized || driver->fd < 0)
    {
      _err("ERROR: SBUS driver not initialized\n");
      return -EINVAL;
    }

  driver->is_running = true;

  while (driver->is_running)
    {
      ret = sbus_driver_read_channels(driver, &channels);
      if (ret == -EAGAIN)
        {
          continue;
        }

      if (ret < 0)
        {
          return ret;
        }

      ret = sbus_lock(driver);
      if (ret < 0)
        {
          return ret;
        }

      callback = driver->channels_callback;
      sbus_unlock(driver);

      if (callback != NULL)
        {
          callback(driver, &channels);
        }
    }

  return OK;
}

void sbus_driver_stop(struct sbus_driver_s *driver)
{
  if (driver == NULL)
    {
      return;
    }

  driver->is_running = false;
}

int sbus_driver_get_latest(struct sbus_driver_s *driver,
                           struct sbus_channels_s *channels)
{
  int ret;

  if (driver == NULL || !driver->is_initialized)
    {
      _err("ERROR: SBUS driver not initialized\n");
      return -EINVAL;
    }

  if (channels == NULL)
    {
      _err("ERROR: SBUS channels pointer is NULL\n");
      return -EINVAL;
    }

  ret = sbus_lock(driver);
  if (ret < 0)
    {
      return ret;
    }

  if (!driver->has_channels)
    {
      sbus_unlock(driver);
      return -EAGAIN;
    }

  memcpy(channels, &driver->latest_channels,
         sizeof(struct sbus_channels_s));

  sbus_unlock(driver);

  return OK;
}

uint32_t sbus_driver_dropped_frames(struct sbus_driver_s *driver)
{
  if (driver == NULL)
    {
      return 0;
    }

  return driver->frame_drops;
}

void sbus_driver_deinit(struct sbus_driver_s *driver)
{
  if (driver == NULL || !driver->is_initialized)
    {
      _err("ERROR: SBUS driver not initialized\n");
      return;
    }

  sbus_driver_stop(driver);

  if (driver->fd >= 0)
    {
      close(driver->fd);
      driver->fd = -1;
    }

  driver->is_initialized = false;
  driver->has_channels   = false;
  sbus_parser_reset(driver);
  sem_destroy(&driver->lock);

  _info("SBUS driver deinitialized\n");
}
