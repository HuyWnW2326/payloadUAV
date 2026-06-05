/****************************************************************************
 * common/mavlink_handler/mavlink_handler.c
 *
 * MAVLink receiver and dispatcher.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <time.h>
#include <poll.h>
#include <math.h>

#include "mavlink_handler.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define READ_BUF_SIZE   128

/* Index pollfd */
#define FD_IDX_UART     0
#define FD_IDX_USB      1
#define MAX_FDS         2

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * UART open
 ****************************************************************************/

static int uart_open(const char *dev, int baud)
{
  struct termios    tio;
  speed_t           speed;
  int               fd;

  fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd < 0)
    {
      printf("[MAVLink] ERROR: Cannot open %s: %d\n", dev, errno);
      return -errno;
    }

  switch (baud)
    {
    case 57600:
      speed = B57600;
      break;

    case 115200:
      speed = B115200;
      break;

    case 921600:
      speed = B921600;
      break;

    default:
      printf("[MAVLink] WARN: Unknown baud %d, defaulting to 57600\n", baud);
      speed = B57600;
      break;
    }

  if (tcgetattr(fd, &tio) < 0)
    {
      printf("[MAVLink] ERROR: tcgetattr failed: %d\n", errno);
      close(fd);
      return -errno;
    }

  cfmakeraw(&tio);
  cfsetispeed(&tio, speed);
  cfsetospeed(&tio, speed);
  tio.c_cflag   |= (CLOCAL | CREAD);
  tio.c_cflag   &= ~CRTSCTS;

  if (tcsetattr(fd, TCSANOW, &tio) < 0)
    {
      printf("[MAVLink] ERROR: tcsetattr failed: %d\n", errno);
      close(fd);
      return -errno;
    }

  tcflush(fd, TCIOFLUSH);
  return fd;
}

/****************************************************************************
 * USB open
 ****************************************************************************/

static int usb_try_open(void)
{
  int fd = open("/dev/ttyACM0", O_RDWR | O_NONBLOCK);

  if (fd >= 0)
    {
      printf("[MAVLink] USB connected -> /dev/ttyACM0\n");
    }
  return fd;
}

/****************************************************************************
 * Low-level send
 ****************************************************************************/

static void mavlink_send(int fd, const uint8_t *buf, uint16_t len)
{
  if (fd < 0 || len == 0)
    {
      return;
    }

  uint16_t sent = 0;

  while (sent < len)
    {
      ssize_t n = write(fd, buf + sent, len - sent);
      if (n < 0)
        {
          if (errno == EAGAIN || errno == EINTR)
            {
              continue;
            }
          break;
        }
      sent += (uint16_t)n;
    }
}

static void mavlink_broadcast(struct mavlink_receiver_s *recv,
                              const uint8_t *buf_uart, uint16_t len_uart,
                              const uint8_t *buf_usb, uint16_t len_usb)
{
  if (recv->uart_fd >= 0)
    {
      mavlink_send(recv->uart_fd, buf_uart, len_uart);
    }
  if (recv->usb_fd >= 0)
    {
      mavlink_send(recv->usb_fd, buf_usb, len_usb);
    }
}

static void send_heartbeat(struct mavlink_receiver_s *recv)
{
  mavlink_message_t msg_uart;
  mavlink_message_t msg_usb;

  uint8_t           buf_uart[MAVLINK_MAX_PACKET_LEN];
  uint16_t          len_uart;
  uint8_t           buf_usb[MAVLINK_MAX_PACKET_LEN];
  uint16_t          len_usb;

  mavlink_msg_heartbeat_pack(1, 204, &msg_uart, MAV_TYPE_ONBOARD_CONTROLLER,
                             MAV_AUTOPILOT_INVALID, 0, 0, MAV_STATE_ACTIVE);

  mavlink_msg_heartbeat_pack(recv->sys_id, recv->comp_id, &msg_usb,
                             MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, 0, 0,
                             MAV_STATE_ACTIVE);

  len_uart  = mavlink_msg_to_send_buffer(buf_uart, &msg_uart);
  len_usb   = mavlink_msg_to_send_buffer(buf_usb, &msg_usb);
  mavlink_broadcast(recv, buf_uart, len_uart, buf_usb, len_usb);
}

static void send_ack(struct mavlink_receiver_s *recv, int reply_fd,
                     uint16_t cmd, uint8_t result)
{
  mavlink_message_t msg;
  uint8_t           buf[MAVLINK_MAX_PACKET_LEN];
  uint16_t          len;

  mavlink_msg_command_ack_pack(recv->sys_id, recv->comp_id, &msg, cmd, result,
                               0, 0, 0, 0);

  len = mavlink_msg_to_send_buffer(buf, &msg);
  mavlink_send(reply_fd, buf, len);
}

static void send_param(struct mavlink_receiver_s *recv, int reply_fd,
                       uint16_t idx)
{
  if (idx >= (uint16_t)NUM_PARAMS)
    {
      return;
    }

  mavlink_message_t msg;
  uint8_t           buf[MAVLINK_MAX_PACKET_LEN];
  uint16_t          len;

  mavlink_msg_param_value_pack(recv->sys_id, recv->comp_id, &msg,
                               g_params[idx].name, g_params[idx].value,
                               MAV_PARAM_TYPE_REAL32, (uint16_t)NUM_PARAMS,
                               idx);

  len = mavlink_msg_to_send_buffer(buf, &msg);
  mavlink_send(reply_fd, buf, len);

  printf("[PARAM] Send [%u/%u] %s = %.1f\n", idx + 1, (unsigned)NUM_PARAMS,
         g_params[idx].name, (double)g_params[idx].value);
}

/****************************************************************************
 * Handle command from QGC
 ****************************************************************************/

static void handle_param_request_list(struct mavlink_receiver_s *recv,
                                      int reply_fd, mavlink_message_t *msg)
{
  mavlink_param_request_list_t req;

  mavlink_msg_param_request_list_decode(msg, &req);

  if (req.target_system != recv->sys_id && req.target_system != 0)
    {
      return;
    }

  printf("[PARAM] Request list -> sending %u params\n", (unsigned)NUM_PARAMS);

  for (uint16_t i = 0; i < (uint16_t)NUM_PARAMS; i++)
    {
      send_param(recv, reply_fd, i);
      usleep(5000);
    }
}

static void handle_param_request_read(struct mavlink_receiver_s *recv,
                                      int reply_fd, mavlink_message_t *msg)
{
  mavlink_param_request_read_t req;

  mavlink_msg_param_request_read_decode(msg, &req);

  if (req.target_system != recv->sys_id && req.target_system != 0)
    {
      return;
    }

  /*
   * MAVLink standard: param_index == -1 means lookup by name.
   * Compare with -1 instead of >= 0 to avoid confusing index 0.
   */
  if (req.param_index != -1)
    {
      send_param(recv, reply_fd, (uint16_t)req.param_index);
      return;
    }

  /* Lookup by name */
  for (uint16_t i = 0; i < (uint16_t)NUM_PARAMS; i++)
    {
      if (strncmp(g_params[i].name, req.param_id, 16) == 0)
        {
          send_param(recv, reply_fd, i);
          return;
        }
    }

  printf("[PARAM] Not found: %.16s\n", req.param_id);
}

static void handle_param_set(struct mavlink_receiver_s *recv, int reply_fd,
                             mavlink_message_t *msg)
{
  mavlink_param_set_t set;

  mavlink_msg_param_set_decode(msg, &set);

  for (uint16_t i = 0; i < (uint16_t)NUM_PARAMS; i++)
    {
      if (strncmp(g_params[i].name, set.param_id, 16) == 0)
        {
          g_params[i].value = set.param_value;

          int ret = param_manager_save();
          if (ret < 0)
            {
              printf("[PARAM] WARNING: Save failed: %d\n", ret);
            }

          send_param(recv, reply_fd, i);
          return;
        }
    }
}

/****************************************************************************
 * Handle command from FC
 ****************************************************************************/

static void handle_param_value(struct mavlink_receiver_s *recv, int reply_fd,
                               mavlink_message_t *msg)
{
  mavlink_param_value_t value;

  mavlink_msg_param_value_decode(msg, &value);

  int idx = param_manager_find(value.param_id);

  if (idx < 0)
    {
      printf("[FC_PARAM] Unknown param from FC: %.16s, ignored\n",
             value.param_id);
      return;
    }

  g_params[idx].value = value.param_value;
}

static void handle_command_long(struct mavlink_receiver_s *recv, int reply_fd,
                                mavlink_message_t *msg)
{
  mavlink_command_long_t cmd;

  mavlink_msg_command_long_decode(msg, &cmd);

  printf(
    "[MAVLink] CMD %u  p1=%.1f p2=%.1f p3=%.1f p4=%.1f p5=%.1f p6=%.1f " "p7=%.1f\n", cmd.command,
    (double)cmd.param1, (double)cmd.param2, (double)cmd.param3, (double)cmd.param4,
    (double)cmd.param5, (double)cmd.param6, (double)cmd.param7);

  uint8_t result;

  switch (cmd.command)
    {
    case MAV_CMD_REQUEST_MESSAGE:
      result = ((uint32_t)cmd.param1 == MAVLINK_MSG_ID_AUTOPILOT_VERSION) ?
               MAV_RESULT_ACCEPTED : MAV_RESULT_UNSUPPORTED;
      break;

    case MAV_CMD_DO_SET_SERVO:
      result = (recv->ops && recv->ops->handle_do_set_servo) ?
               recv->ops->handle_do_set_servo(recv, &cmd) :
               MAV_RESULT_UNSUPPORTED;
      break;

    case MAV_CMD_DO_SET_ACTUATOR:
      result = (recv->ops && recv->ops->handle_do_set_actuator) ?
               recv->ops->handle_do_set_actuator(recv, &cmd) :
               MAV_RESULT_UNSUPPORTED;
      break;

    default:
      result = MAV_RESULT_UNSUPPORTED;
      break;
    }

  send_ack(recv, reply_fd, cmd.command, result);
}

static void handle_mission_request_list(struct mavlink_receiver_s *recv,
                                        int reply_fd, mavlink_message_t *msg)
{
  mavlink_mission_request_list_t req;

  mavlink_msg_mission_request_list_decode(msg, &req);

  if (req.target_system != recv->sys_id && req.target_system != 0)
    {
      return;
    }

  printf("[MAVLink] Mission request list -> sending empty list\n");

  mavlink_message_t rsp;
  uint8_t           buf[MAVLINK_MAX_PACKET_LEN];
  uint16_t          len;

  mavlink_msg_mission_count_pack(recv->sys_id, recv->comp_id, &rsp,
                                 req.target_system, req.target_component, 0,
                                 MAV_MISSION_TYPE_MISSION, 0);

  len = mavlink_msg_to_send_buffer(buf, &rsp);
  mavlink_send(reply_fd, buf, len);
}

/****************************************************************************
 * Request param
 ****************************************************************************/

static void request_param(struct mavlink_receiver_s *recv,
                          const char *param_name)
{
  mavlink_message_t msg;
  uint8_t           buffer[MAVLINK_MAX_PACKET_LEN];
  uint16_t          len;

  mavlink_msg_param_request_read_pack(recv->sys_id, recv->comp_id, &msg, 1,
                                      MAV_COMP_ID_AUTOPILOT1, param_name, -1);

  len = mavlink_msg_to_send_buffer(buffer, &msg);
  mavlink_send(recv->uart_fd, buffer, len);
}

/****************************************************************************
 * Handle MESSAGE — dispatcher
 ****************************************************************************/

static void dispatch_message(struct mavlink_receiver_s *recv, int reply_fd,
                             mavlink_message_t *msg)
{
  printf("[MAVLink] RX msgid=%-4u sysid=%u compid=%u\n", msg->msgid,
         msg->sysid, msg->compid);

  switch (msg->msgid)
    {
    case MAVLINK_MSG_ID_COMMAND_LONG:
      handle_command_long(recv, reply_fd, msg);
      break;

    case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
      handle_param_request_list(recv, reply_fd, msg);
      break;

    case MAVLINK_MSG_ID_PARAM_REQUEST_READ:
      handle_param_request_read(recv, reply_fd, msg);
      break;

    case MAVLINK_MSG_ID_PARAM_SET:
      handle_param_set(recv, reply_fd, msg);
      break;

    case MAVLINK_MSG_ID_MISSION_REQUEST_LIST:
      handle_mission_request_list(recv, reply_fd, msg);
      break;

    case MAVLINK_MSG_ID_PARAM_VALUE:
      handle_param_value(recv, reply_fd, msg);
      break;

    case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
    if (recv->ops && recv->ops->handle_local_position)
        recv->ops->handle_local_position(recv, reply_fd, msg);
    break;

    case MAVLINK_MSG_ID_HEARTBEAT:
    if (recv->ops && recv->ops->handle_heartbeat)
        recv->ops->handle_heartbeat(recv, reply_fd, msg);
    break;

    default:
      break;
    }
}

/****************************************************************************
 * Handle Helpers
 ****************************************************************************/

static uint64_t get_time_ms(void)
{
  struct timespec ts;

  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

static void process_fd(int fd, uint8_t channel, mavlink_message_t *msg,
                       mavlink_status_t *status,
                       struct mavlink_receiver_s *recv, int reply_fd)
{
  uint8_t   buf[READ_BUF_SIZE];
  ssize_t   nread;

  while ((nread = read(fd, buf, sizeof(buf))) > 0)
    {
      for (ssize_t i = 0; i < nread; i++)
        {
          if (mavlink_parse_char(channel, buf[i], msg, status))
            {
              dispatch_message(recv, reply_fd, msg);
            }
        }
    }
}

/****************************************************************************
 * Main loop
 ****************************************************************************/

int run(struct mavlink_receiver_s *recv)
{
  mavlink_message_t msg_uart;
  mavlink_message_t msg_usb;
  mavlink_status_t  status_uart;
  mavlink_status_t  status_usb;

  uint64_t          last_hb_ms      = get_time_ms();
  uint64_t          last_usb_retry  = get_time_ms();

  memset(&status_uart, 0, sizeof(status_uart));
  memset(&status_usb, 0, sizeof(status_usb));
  memset(&msg_uart, 0, sizeof(msg_uart));
  memset(&msg_usb, 0, sizeof(msg_usb));

  recv->is_running = true;
  printf("[MAVLink] Running  sysid=%d compid=%d  params=%u\n", recv->sys_id,
         recv->comp_id, (unsigned)NUM_PARAMS);

  while (recv->is_running)
    {
      uint64_t now = get_time_ms();

      /* --- Retry open USB --- */
      if (recv->usb_fd < 0 && (now - last_usb_retry >= 2000))
        {
          recv->usb_fd      = usb_try_open();
          last_usb_retry    = now;
        }

      /* --- Build pollfd array --- */
      struct pollfd fds[MAX_FDS];
      int           nfds = 0;

      fds[FD_IDX_UART].fd       = recv->uart_fd;
      fds[FD_IDX_UART].events   = POLLIN;
      nfds++;

      bool usb_active = (recv->usb_fd >= 0);
      if (usb_active)
        {
          fds[FD_IDX_USB].fd        = recv->usb_fd;
          fds[FD_IDX_USB].events    = POLLIN;
          nfds++;
        }

      /* --- Heartbeat 1Hz --- */
      if (now - last_hb_ms >= 1000)
        {
          send_heartbeat(recv);
          last_hb_ms = now;
        }

      int ret = poll(fds, (nfds_t)nfds, 50); /* 50ms timeout */
      if (ret <= 0)
        {
          continue;
        }

      /* --- UART (MAVLINK_COMM_0) --- */
      if (fds[FD_IDX_UART].revents & POLLIN)
        {
          process_fd(recv->uart_fd, MAVLINK_COMM_0, &msg_uart, &status_uart,
                     recv, recv->uart_fd);
        }

      /* --- USB (MAVLINK_COMM_1) --- */
      if (usb_active)
        {
          if (fds[FD_IDX_USB].revents & POLLIN)
            {
              process_fd(recv->usb_fd, MAVLINK_COMM_1, &msg_usb, &status_usb,
                         recv, recv->usb_fd);
            }
          else if (fds[FD_IDX_USB].revents & (POLLERR | POLLHUP))
            {
              printf("[MAVLink] USB disconnected\n");
              close(recv->usb_fd);
              recv->usb_fd      = -1;
              last_usb_retry    = now;
              memset(&status_usb, 0, sizeof(status_usb));
            }
        }
    }

  return 0;
}

/****************************************************************************
 * Init / Deinit
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int mavlink_receiver_init(struct mavlink_receiver_s *recv,
                          const char *uart_dev, int baud,
                          struct pwm_driver_s *pwm, mavlink_ops_t *ops)
{
  memset(recv, 0, sizeof(*recv));
  recv->sys_id  = (uint8_t)PARAM_GET_U32(PARAM_MAV_SYS_ID);
  recv->comp_id = (uint8_t)PARAM_GET_U32(PARAM_MAV_COMP_ID);
  recv->pwm     = pwm;
  recv->ops     = ops;
  recv->usb_fd  = -1;

  recv->uart_fd = uart_open(uart_dev, baud);
  if (recv->uart_fd < 0)
    {
      return recv->uart_fd;
    }
  printf("[MAVLink] UART OK  dev=%s baud=%d\n", uart_dev, baud);

  recv->usb_fd = usb_try_open();
  if (recv->usb_fd < 0)
    {
      printf("[MAVLink] USB not ready, will retry every 2s\n");
    }

  return 0;
}

void mavlink_receiver_stop(struct mavlink_receiver_s *recv)
{
  recv->is_running = false;
}

void mavlink_receiver_deinit(struct mavlink_receiver_s *recv)
{
  if (recv->uart_fd >= 0)
    {
      close(recv->uart_fd);
      recv->uart_fd = -1;
    }

  if (recv->usb_fd >= 0)
    {
      close(recv->usb_fd);
      recv->usb_fd = -1;
    }
}
