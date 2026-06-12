/****************************************************************************
 * payloads/winch/winch_main.c
 *
 * Winch payload entry point.
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
} winch_receiver_t;

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct pwm_driver_s   g_pwm;
static winch_receiver_t      g_mavlink;
static bool                  g_pwm_initialized       = false;
static bool                  g_mavlink_initialized   = false;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static uint8_t handle_do_set_actuator(struct mavlink_receiver_s *recv,
                                      const mavlink_command_long_t *cmd)
{

  winch_receiver_t *winch = (winch_receiver_t *)recv;

  uint8_t duty = 0;

  if (winch->mavlink.pwm == NULL)
    {
      return MAV_RESULT_FAILED;
    }

  uint8_t result = MAV_RESULT_ACCEPTED;

  if(cmd->param1 > 0)
    duty = 100;
  else 
    duty  = 0;

  int r = pwm_driver_set_duty(winch->mavlink.pwm, 1, duty);
  printf("[MAVLink] Actuator -> %d%%\n", duty);

  if (r < 0)
    {
       result = MAV_RESULT_FAILED;
    }

  return result;
}

static mavlink_ops_t winch_ops = {
  .handle_do_set_servo      = NULL,
  .handle_do_set_actuator   = handle_do_set_actuator,
  .handle_heartbeat         = NULL,
  .handle_local_position    = NULL
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
    pwm_driver_add_channel(&g_pwm, "/dev/pwm0", 2, PWM_MODE_DUTY, 0, 50);
  if (ret < 0)
    {
      goto err;
    }
  printf("✓ Servo 1 (TIM2-CH1)");

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
                                  MAVLINK_UART_BAUDRATE, &g_pwm, &winch_ops);

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

int winch_payload_main(int argc, char *argv[])
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
