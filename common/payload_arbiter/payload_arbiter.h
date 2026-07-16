#ifndef __COMMON_PAYLOAD_ARBITER_H
#define __COMMON_PAYLOAD_ARBITER_H

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum
{
  PAYLOAD_MODE_MANUAL_SBUS = 0,
  PAYLOAD_MODE_AUTO_MAVLINK,
  PAYLOAD_MODE_FAILSAFE_LOCKED
} payload_mode_t;

typedef enum
{
  PAYLOAD_SOURCE_SBUS = 0,
  PAYLOAD_SOURCE_MAVLINK
} payload_source_t;

typedef void (*payload_execute_cb_t)(void *arg, payload_source_t source);

typedef struct
{
  pthread_mutex_t       mutex;
  sem_t                 sem;

  payload_mode_t        current_mode;
  bool                  trigger_requested;
  bool                  executed;
  payload_source_t      source;

  payload_execute_cb_t  on_execute;
  void                 *execute_arg;
} payload_arbiter_t;

int  payload_arbiter_init(payload_arbiter_t *arb,
                          payload_execute_cb_t on_execute, void *arg);
void payload_arbiter_set_mode(payload_arbiter_t *arb, payload_mode_t mode);
void payload_arbiter_try_trigger(payload_arbiter_t *arb,
                                 payload_source_t source);
void payload_arbiter_rearm(payload_arbiter_t *arb);
int  payload_arbiter_executor_entry(payload_arbiter_t *arb);

#endif /* __COMMON_PAYLOAD_ARBITER_H */