/****************************************************************************
 * common/sbus/sbus_driver.c
 *
 * Minimal SBUS Driver Implementation
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <debug.h>
#include <string.h>
#include <poll.h>

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

static int sbus_read_byte(struct sbus_driver_s *driver, uint8_t *byte)
{
  struct pollfd pfd;
  ssize_t nread;
  int ret;

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

  fd = open(devpath, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd < 0)
    {
      int errcode = errno;
      _err("ERROR: Failed to open %s: %d\n", devpath, errcode);
      return -errcode;
    }

  ret = sbus_config_uart(fd);
  if (ret < 0)
    {
      close(fd);
      return ret;
    }

  driver->fd             = fd;
  driver->is_initialized = true;

  _info("SBUS driver initialized: %s\n", devpath);

  return OK;
}

int sbus_driver_read_frame(struct sbus_driver_s *driver,
                           struct sbus_frame_s *frame)
{
  uint8_t byte;
  int idx;
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

  do
    {
      ret = sbus_read_byte(driver, &byte);
      if (ret < 0)
        {
          return ret;
        }
    }
  while (byte != SBUS_START_BYTE);

  frame->data[0] = byte;

  for (idx = 1; idx < SBUS_FRAME_SIZE; idx++)
    {
      ret = sbus_read_byte(driver, &frame->data[idx]);
      if (ret < 0)
        {
          return ret;
        }
    }

  return OK;
}

int sbus_driver_decode_frame(const struct sbus_frame_s *frame,
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

  return sbus_driver_decode_frame(&frame, channels);
}

int sbus_driver_run(struct sbus_driver_s *driver)
{
  struct sbus_channels_s channels;
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

      driver->latest_channels = channels;
      driver->has_channels    = true;
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

  _info("SBUS driver deinitialized\n");
}
