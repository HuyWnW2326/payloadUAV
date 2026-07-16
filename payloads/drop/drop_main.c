/****************************************************************************
 * payloads/drop/drop_main.c
 *
 * Drop payload entry point.
 *
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <math.h>

#include "pwm/pwm_driver.h"
#include "mavlink_handler/mavlink_handler.h"
#include "sbus/sbus_channel_task.h"
#include "payload_arbiter/payload_arbiter.h"
#include "param/param.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MAVLINK_TASK_STACK      4096
#define MAVLINK_TASK_PRIO       100
#define MAVLINK_UART_DEVICE     "/dev/ttyS1"
#define MAVLINK_UART_BAUDRATE   57600

#define SBUS_TASK_STACK         2048
#define SBUS_TASK_PRIO          100
#define SBUS_UART_DEVICE        "/dev/ttyS2"

#define EXECUTOR_TASK_STACK     2048
#define EXECUTOR_TASK_PRIO      120   /* cao hơn SBUS/MAVLink task */

/* SBUS channel mapping cho drop trigger — chỉnh theo transmitter thật */

#define SBUS_CH_MODE            5
#define SBUS_CH_DROP_TRIGGER     6
#define SBUS_THRESHOLD           1200
#define SBUS_DEBOUNCE_N          3
#define SBUS_FAILSAFE_LIMIT      10

/* Servo index dùng riêng cho drop trigger, tách khỏi 6 kênh actuator
 * proportional đã có trong handle_do_set_actuator.
 */

#define DROP_SERVO_IDX           5   /* VD: dùng servo 6, servo 1-5 vẫn free cho actuator cmd */
#define DROP_SERVO_FIRE_PULSE_US 2000
#define DROP_SERVO_REST_PULSE_US 1000

/****************************************************************************
 * Private Types
 ****************************************************************************/

typedef struct
{
  struct mavlink_receiver_s mavlink;
} drop_receiver_t;

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct pwm_driver_s   g_pwm;
static drop_receiver_t       g_mavlink;
static payload_arbiter_t     g_arb;
static bool                  g_pwm_initialized       = false;
static bool                  g_mavlink_initialized   = false;

/****************************************************************************
 * Private Functions — giữ nguyên 100% từ bản gốc
 ****************************************************************************/

static uint8_t handle_do_set_actuator(struct mavlink_receiver_s *recv,
                                      const mavlink_command_long_t *cmd)
{
  drop_receiver_t *drop = (drop_receiver_t *)recv;

  if (drop->mavlink.pwm == NULL)
    {
      return MAV_RESULT_FAILED;
    }

  float params[6] =
    {
      cmd->param1, cmd->param2, cmd->param3,
      cmd->param4, cmd->param5, cmd->param6
    };

  uint8_t result = MAV_RESULT_ACCEPTED;

  for (int i = 0; i < 6; i++)
    {
      if (isnan(params[i]))
        {
          continue;
        }

      float    norm      = fmaxf(-1.0f, fminf(1.0f, params[i]));
      uint32_t min_pulse = PARAM_GET_U32(PARAM_SERVO1_MIN + i * 2);
      uint32_t max_pulse = PARAM_GET_U32(PARAM_SERVO1_MIN + i * 2 + 1);
      uint32_t pulse     = (uint32_t)(min_pulse +
                           (norm + 1.0f) * 0.5f *
                           (max_pulse - min_pulse));

      int r = pwm_driver_set_pulse(drop->mavlink.pwm, (uint8_t)i, pulse);
      printf("[MAVLink] Actuator %d -> %lu us [%s]\n",
             i + 1, (unsigned long)pulse, r >= 0 ? "OK" : "FAIL");

      if (r < 0)
        {
          result = MAV_RESULT_FAILED;
        }
    }

  return result;
}

/****************************************************************************
 * Private Functions — MỚI: drop trigger qua arbiter
 ****************************************************************************/

/* MAV_CMD_DO_SET_SERVO -> chỉ dùng cho kênh drop trigger, không phải trim.
 * cmd->param1 = servo number (theo chuẩn MAVLink, 1-indexed).
 */

static uint8_t handle_do_set_servo(struct mavlink_receiver_s *recv,
                                   const mavlink_command_long_t *cmd)
{
  uint8_t servo_num = (uint8_t)cmd->param1;

  if (servo_num != (DROP_SERVO_IDX + 1))
    {
      return MAV_RESULT_UNSUPPORTED;
    }

  payload_arbiter_try_trigger(&g_arb, PAYLOAD_SOURCE_MAVLINK);
  return MAV_RESULT_ACCEPTED;
}

/* Gọi bởi executor task, NGOÀI mutex lock — hành động servo thật */

static void drop_execute_cb(void *arg, payload_source_t source)
{
  struct pwm_driver_s *pwm = (struct pwm_driver_s *)arg;
  int r = pwm_driver_set_pulse(pwm, DROP_SERVO_IDX, DROP_SERVO_FIRE_PULSE_US);

  printf("[Drop] FIRE (source=%s) -> %d us [%s]\n",
         source == PAYLOAD_SOURCE_SBUS ? "SBUS" : "MAVLink",
         DROP_SERVO_FIRE_PULSE_US, r >= 0 ? "OK" : "FAIL");
}

static void on_mode_level(void *arg, uint16_t value)
{
  payload_mode_t mode = (value > SBUS_THRESHOLD) ?
                         PAYLOAD_MODE_MANUAL_SBUS : PAYLOAD_MODE_AUTO_MAVLINK;
  payload_arbiter_set_mode(&g_arb, mode);
}

static void on_trigger_edge(void *arg)
{
  payload_arbiter_try_trigger(&g_arb, PAYLOAD_SOURCE_SBUS);
}

static void on_sbus_failsafe(void *arg, bool active)
{
  if (active)
    {
      payload_arbiter_set_mode(&g_arb, PAYLOAD_MODE_FAILSAFE_LOCKED);
      printf("[SBUS] Failsafe active -> drop locked\n");
    }
}

static struct sbus_edge_watch_s g_edges[] =
{
  {
    .channel    = SBUS_CH_DROP_TRIGGER,
    .threshold  = SBUS_THRESHOLD,
    .debounce_n = SBUS_DEBOUNCE_N,
    .on_edge    = on_trigger_edge,
    .cb_arg     = NULL,
  },
};

static struct sbus_level_watch_s g_levels[] =
{
  {
    .channel  = SBUS_CH_MODE,
    .on_level = on_mode_level,
    .cb_arg   = NULL,
  },
};

static struct sbus_channel_task_config_s g_sbus_cfg =
{
  .devpath                = SBUS_UART_DEVICE,
  .edge_watches           = g_edges,
  .num_edge_watches       = 1,
  .level_watches          = g_levels,
  .num_level_watches      = 1,
  .on_failsafe            = on_sbus_failsafe,
  .failsafe_arg           = NULL,
  .failsafe_eagain_limit  = SBUS_FAILSAFE_LIMIT,
};

static const mavlink_ops_t drop_ops =
{
  .handle_do_set_servo      = handle_do_set_servo,
  .handle_do_set_actuator   = handle_do_set_actuator,
  .handle_heartbeat         = NULL,
  .handle_local_position    = NULL
};

/****************************************************************************
 * Task entry points
 ****************************************************************************/

static int mavlink_task_entry(int argc, char *argv[])
{
  return mavlink_receiver_run(&g_mavlink.mavlink);
}

static int sbus_task_entry(int argc, char *argv[])
{
  return sbus_channel_task_run(&g_sbus_cfg);
}

static int executor_task_entry(int argc, char *argv[])
{
  return payload_arbiter_executor_entry(&g_arb);
}

/****************************************************************************
 * Init helpers — giữ nguyên init_pwm() và init_mavlink() từ bản gốc
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

  ret = pwm_driver_add_channel(&g_pwm, "/dev/pwm0", 1, PWM_MODE_SERVO,
                               PARAM_GET_U32(PARAM_SERVO1_MIN), 0);
  if (ret < 0) goto err;
  printf("✓ Servo 1 (TIM2-CH1)");

  ret = pwm_driver_add_channel(&g_pwm, "/dev/pwm0", 2, PWM_MODE_SERVO,
                               PARAM_GET_U32(PARAM_SERVO2_MIN), 0);
  if (ret < 0) goto err;
  printf("✓ Servo 2 (TIM2-CH2)");

  ret = pwm_driver_add_channel(&g_pwm, "/dev/pwm1", 1, PWM_MODE_SERVO,
                               PARAM_GET_U32(PARAM_SERVO3_MIN), 0);
  if (ret < 0) goto err;
  printf("✓ Servo 3 (TIM3-CH1)");

  ret = pwm_driver_add_channel(&g_pwm, "/dev/pwm1", 2, PWM_MODE_SERVO,
                               PARAM_GET_U32(PARAM_SERVO4_MIN), 0);
  if (ret < 0) goto err;
  printf("✓ Servo 4 (TIM3-CH2)\n");

  ret = pwm_driver_add_channel(&g_pwm, "/dev/pwm1", 3, PWM_MODE_SERVO,
                               PARAM_GET_U32(PARAM_SERVO5_MIN), 0);
  if (ret < 0) goto err;
  printf("✓ Servo 5 (TIM3-CH3) \n");

  /* Servo 6 = kênh drop trigger, dùng DROP_SERVO_REST_PULSE_US làm vị trí nghỉ */

  ret = pwm_driver_add_channel(&g_pwm, "/dev/pwm1", 4, PWM_MODE_SERVO,
                               DROP_SERVO_REST_PULSE_US, 0);
  if (ret < 0) goto err;
  printf("✓ Servo 6 (TIM3-CH4) [DROP TRIGGER]\n");

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
                                  MAVLINK_UART_BAUDRATE, &g_pwm, &drop_ops);

  if (ret < 0)
    {
      printf("ERROR: MAVLink init failed: %d\n", ret);
    }
  return ret;
}

/****************************************************************************
 * Main
 ****************************************************************************/

int drop_payload_main(int argc, char *argv[])
{
  int ret;

  printf("\n=== Payload Controller ===\n\n");

  param_manager_init();

  ret = init_pwm();
  if (ret < 0)
    {
      printf("ERROR: PWM init failed: %d\n", ret);
      return ret;
    }
  g_pwm_initialized = true;
  printf("✓ PWM OK\n\n");

  /* Arbiter init TRƯỚC executor task, để mutex/sem sẵn sàng */

  payload_arbiter_init(&g_arb, drop_execute_cb, &g_pwm);

  ret = init_mavlink();
  if (ret < 0)
    {
      goto cleanup;
    }
  g_mavlink_initialized = true;
  printf("✓ MAVLink OK\n\n");

  /* Executor tạo trước MAVLink/SBUS task, để sẵn sàng nhận sem_post ngay
   * khi có trigger đầu tiên.
   */

  ret = task_create("drop_executor", EXECUTOR_TASK_PRIO, EXECUTOR_TASK_STACK,
                    executor_task_entry, NULL);
  if (ret < 0)
    {
      printf("ERROR: Cannot create executor task: %d\n", ret);
      goto cleanup;
    }
  printf("✓ Executor task started\n");

  ret = task_create("mavlink_recv", MAVLINK_TASK_PRIO, MAVLINK_TASK_STACK,
                    mavlink_task_entry, NULL);
  if (ret < 0)
    {
      printf("ERROR: Cannot create mavlink task: %d\n", ret);
      goto cleanup;
    }
  printf("✓ MAVLink task started\n");

  ret = task_create("sbus_recv", SBUS_TASK_PRIO, SBUS_TASK_STACK,
                    sbus_task_entry, NULL);
  if (ret < 0)
    {
      printf("ERROR: Cannot create sbus task: %d\n", ret);
      goto cleanup;
    }
  printf("✓ SBUS task started\n");

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