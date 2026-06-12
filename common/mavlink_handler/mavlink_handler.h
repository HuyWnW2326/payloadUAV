/****************************************************************************
 * common/mavlink_handler/mavlink_handler.h
 *
 * MAVLink receiver interface.
 *
 ****************************************************************************/

#ifndef __COMMON_MAVLINK_HANDLER_H
#define __COMMON_MAVLINK_HANDLER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <mavlink/common/mavlink.h>
#include "pwm/pwm_driver.h"
#include "param/param.h"

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct mavlink_receiver_s;
struct pwm_driver_s;

typedef struct
{
  uint8_t (*handle_do_set_servo)(struct mavlink_receiver_s *recv,
                                 const mavlink_command_long_t *cmd);

  uint8_t (*handle_do_set_actuator)(struct mavlink_receiver_s *recv,
                                    const mavlink_command_long_t *cmd);

  void (*handle_local_position)(struct mavlink_receiver_s *recv,
                                int reply_fd, mavlink_message_t *msg);

  void (*handle_heartbeat)(struct mavlink_receiver_s *recv,
                           int reply_fd, mavlink_message_t *msg);
} mavlink_ops_t;

struct mavlink_receiver_s
{
  int uart_fd;
  int usb_fd;
  uint8_t sys_id;
  uint8_t comp_id;
  volatile bool is_running;
  struct pwm_driver_s *pwm;
  mavlink_ops_t *ops;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

int mavlink_receiver_init(struct mavlink_receiver_s *recv,
                          const char *uart_dev, int baud,
                          struct pwm_driver_s *pwm, mavlink_ops_t *ops);

int mavlink_receiver_run(struct mavlink_receiver_s *recv);

void mavlink_receiver_stop(struct mavlink_receiver_s *recv);

void mavlink_receiver_deinit(struct mavlink_receiver_s *recv);

#endif /* __COMMON_MAVLINK_HANDLER_H */
