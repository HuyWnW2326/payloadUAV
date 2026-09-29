/****************************************************************************
 * payloads/agriculture/agricultural_main.c
 *
 * Agriculture payload entry point.
 *
 ****************************************************************************/
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <math.h>

#include "pwm/pwm_driver.h"
#include "mavlink_handler/mavlink_handler.h"
#include "param/param.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MAVLINK_TASK_STACK      4096
#define MAVLINK_TASK_PRIO       100
#define MAVLINK_UART_DEVICE     "/dev/ttyS1"
#define MAVLINK_UART_BAUDRATE   57600

/****************************************************************************
 * Private Types
 ****************************************************************************/

typedef struct
{
  struct mavlink_receiver_s mavlink;
  float density;
  float spray_width;
  bool is_spraying;
  bool is_mission_mode;
  bool test_mode;
} agriculture_receiver_t;

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct pwm_driver_s      g_pwm;
static agriculture_receiver_t   g_mavlink;
static bool                     g_pwm_initialized       = false;
static bool                     g_mavlink_initialized   = false;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void set_all_pwm_min(struct pwm_driver_s *pwm)
{
  const uint32_t pulse[] = {
    PARAM_GET_U32(PARAM_SERVO1_MIN), PARAM_GET_U32(PARAM_SERVO2_MIN),
    PARAM_GET_U32(PARAM_SERVO3_MIN), PARAM_GET_U32(PARAM_SERVO4_MIN),
    PARAM_GET_U32(PARAM_SERVO5_MIN), PARAM_GET_U32(PARAM_SERVO6_MIN)
  };

  pwm_driver_set_all_pulses(pwm, pulse, 6);
}

static void set_all_pwm_max(struct pwm_driver_s *pwm)
{
  const uint32_t pulse[] = {
    PARAM_GET_U32(PARAM_SERVO1_MAX), PARAM_GET_U32(PARAM_SERVO2_MAX),
    PARAM_GET_U32(PARAM_SERVO3_MAX), PARAM_GET_U32(PARAM_SERVO4_MAX),
    PARAM_GET_U32(PARAM_SERVO5_MAX), PARAM_GET_U32(PARAM_SERVO6_MAX)
  };

  pwm_driver_set_all_pulses(pwm, pulse, 6);
}

static void set_spray_pwm(struct pwm_driver_s *pwm, uint32_t pump_pulse)
{
  const uint32_t pulse[] = {
    PARAM_GET_U32(PARAM_SERVO1_MAX), PARAM_GET_U32(PARAM_SERVO2_MAX),
    PARAM_GET_U32(PARAM_SERVO3_MAX), PARAM_GET_U32(PARAM_SERVO4_MAX),
    pump_pulse, PARAM_GET_U32(PARAM_SERVO6_MIN)
  };

  pwm_driver_set_all_pulses(pwm, pulse, 6);
}

static uint8_t handle_do_set_actuator(struct mavlink_receiver_s *recv,
                                      const mavlink_command_long_t *cmd)
{

  agriculture_receiver_t *agri = (agriculture_receiver_t *)recv;

  if (agri->mavlink.pwm == NULL)
    {
      return MAV_RESULT_FAILED;
    }

  if (cmd->param2 > 0)
    {
      agri->test_mode = true;
      set_all_pwm_max(agri->mavlink.pwm);
    }
  else if (cmd->param2 < 0)
    {
      agri->test_mode = true;
      set_all_pwm_min(agri->mavlink.pwm);
    }
  else
    {
      agri->test_mode = false;
      agri->is_spraying = (cmd->param1 > 0);

      if (!agri->is_spraying)
        {
          set_all_pwm_min(agri->mavlink.pwm);
        }
    }

  return MAV_RESULT_ACCEPTED;
}

static void handle_local_position(struct mavlink_receiver_s *recv, 
                                  int reply_fd, mavlink_message_t *msg)
{
  agriculture_receiver_t *agri = (agriculture_receiver_t *)recv;

  mavlink_local_position_ned_t pos;
  mavlink_msg_local_position_ned_decode(msg, &pos);

  float speed = sqrtf(pos.vx * pos.vx + pos.vy * pos.vy);

  if (agri->test_mode)
    {
      return;
    }

  if (agri->is_mission_mode && agri->is_spraying && speed > 0.5f)
    {
      float flow_rate = (agri->density * speed * agri->spray_width) *
                        (60.0f / 10000.0f);
      float pump_pulse = 1100.0f + 25.8f * (flow_rate - 1.0f);

      if (pump_pulse > 1100.0f)
        {
          set_spray_pwm(recv->pwm, (uint32_t)pump_pulse);
        }
      else
        {
          set_all_pwm_min(recv->pwm);
        }
    }
  else
    {
      set_all_pwm_min(recv->pwm);
    }
}

static void handle_heartbeat(struct mavlink_receiver_s *recv, 
                             int reply_fd, mavlink_message_t *msg)
{
  mavlink_heartbeat_t heartbeat;
  mavlink_msg_heartbeat_decode(msg, &heartbeat);

  agriculture_receiver_t *agri = (agriculture_receiver_t *)recv;

  uint32_t custom_mode = heartbeat.custom_mode;

  uint8_t main_mode = (custom_mode >> 16) & 0xFF;
  uint8_t sub_mode  = (custom_mode >> 24) & 0xFF;

  if (main_mode == 4 && sub_mode == 4)
    {
      agri->is_mission_mode = true;
    }
  else
    {
      agri->is_mission_mode = false;
    }
}

static const mavlink_ops_t agriculture_ops = {
  .handle_do_set_servo      = NULL,
  .handle_do_set_actuator   = handle_do_set_actuator,
  .handle_heartbeat         = handle_heartbeat,
  .handle_local_position    = handle_local_position
};

/****************************************************************************
 * MAVLink task entry point
 ****************************************************************************/

static int mavlink_task_entry(int argc, char *argv[])
{
  return mavlink_receiver_run(&g_mavlink.mavlink);
}

/****************************************************************************
 * Init helpers
 ****************************************************************************/

static int init_pwm(void)
{
  int ret;

  ret = pwm_driver_init(&g_pwm, PARAM_GET_U32(PARAM_PWM_FREQ));
  if (ret < 0)
    {
      printf("ERROR: PWM init failed: %d\n", ret);
      return ret;
    }

  /* TIM2 */
  ret =
    pwm_driver_add_channel(&g_pwm, "/dev/pwm0", 1, PWM_MODE_SERVO, PARAM_GET_U32(PARAM_SERVO1_MIN), 0);
  if (ret < 0)
    {
      goto err;
    }
  printf("✓ Servo 1 (TIM2-CH1)");

  ret =
    pwm_driver_add_channel(&g_pwm, "/dev/pwm0", 2, PWM_MODE_SERVO, PARAM_GET_U32(PARAM_SERVO2_MIN), 0);
  if (ret < 0)
    {
      goto err;
    }
  printf("✓ Servo 2 (TIM2-CH2)");

  /* TIM3 */
  ret =
    pwm_driver_add_channel(&g_pwm, "/dev/pwm1", 1, PWM_MODE_SERVO, PARAM_GET_U32(PARAM_SERVO3_MIN), 0);
  if (ret < 0)
    {
      goto err;
    }
  printf("✓ Servo 3 (TIM3-CH1)");

  ret =
    pwm_driver_add_channel(&g_pwm, "/dev/pwm1", 2, PWM_MODE_SERVO, PARAM_GET_U32(PARAM_SERVO4_MIN), 0);
  if (ret < 0)
    {
      goto err;
    }
  printf("✓ Servo 4 (TIM3-CH2)\n");

  ret =
    pwm_driver_add_channel(&g_pwm, "/dev/pwm1", 3, PWM_MODE_SERVO, PARAM_GET_U32(PARAM_SERVO5_MIN), 0);
  if (ret < 0)
    {
      goto err;
    }
  printf("✓ Servo 5 (TIM3-CH3) \n");

  ret =
    pwm_driver_add_channel(&g_pwm, "/dev/pwm1", 4, PWM_MODE_SERVO, PARAM_GET_U32(PARAM_SERVO6_MIN), 0);
  if (ret < 0)
    {
      goto err;
    }
  printf("✓ Servo 6 (TIM3-CH4)\n");

  return 0;

err:
  printf("ERROR: Add channel failed: %d\n", ret);
  pwm_driver_stop_all(&g_pwm);
  pwm_driver_deinit(&g_pwm);
  return ret;
}

static int init_mavlink(void)
{
  int ret = mavlink_receiver_init(&g_mavlink.mavlink, MAVLINK_UART_DEVICE, 
                                  MAVLINK_UART_BAUDRATE, &g_pwm, &agriculture_ops);

  if (ret < 0)
    {
      printf("ERROR: MAVLink init failed: %d\n", ret);
    }
  return ret;
}

/****************************************************************************
 * Main
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int stewart_payload_main(int argc, char *argv[])
{
  int ret;

  printf("\n=== Payload Controller ===\n\n");

  /* Init Param */
  param_manager_init();

  /* Init PWM */
  ret = init_pwm();
  if (ret < 0)
    {
      printf("ERROR: PWM init failed: %d\n", ret);
      return ret;
    }
  g_pwm_initialized = true;
  printf("✓ PWM OK\n\n");

  /* Init MAVLink */
  ret = init_mavlink();
  if (ret < 0)
    {
      goto cleanup;
    }
  g_mavlink_initialized = true;
  printf("✓ MAVLink OK\n\n");

  /* Spawn MAVLink task */
  ret = task_create("mavlink_recv", MAVLINK_TASK_PRIO, MAVLINK_TASK_STACK,
                    mavlink_task_entry, NULL);
  if (ret < 0)
    {
      printf("ERROR: Cannot create mavlink task: %d\n", ret);
      goto cleanup;
    }
  printf("✓ MAVLink task started\n");

  printf(
    "\n[Payload] ---------------- All systems running ---------------\n\n");

  while (1)
    {
      sleep(1);
    }

cleanup:
  if (g_mavlink_initialized)
    {
      mavlink_receiver_deinit(&g_mavlink.mavlink);
      g_mavlink_initialized = false;
    }

  if (g_pwm_initialized)
    {
      pwm_driver_stop_all(&g_pwm);
      pwm_driver_deinit(&g_pwm);
      g_pwm_initialized = false;
    }

  printf("✓ Cleanup done\n\n");
  return 0;
}
