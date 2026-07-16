#include "payload_arbiter.h"
#include <string.h>

int payload_arbiter_init(payload_arbiter_t *arb,
                         payload_execute_cb_t on_execute, void *arg)
{
  pthread_mutexattr_t attr;

  memset(arb, 0, sizeof(*arb));
  arb->current_mode = PAYLOAD_MODE_FAILSAFE_LOCKED;
  arb->on_execute    = on_execute;
  arb->execute_arg   = arg;

  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
  pthread_mutex_init(&arb->mutex, &attr);
  sem_init(&arb->sem, 0, 0);

  return 0;
}

void payload_arbiter_set_mode(payload_arbiter_t *arb, payload_mode_t mode)
{
  pthread_mutex_lock(&arb->mutex);
  arb->current_mode = mode;
  pthread_mutex_unlock(&arb->mutex);
}

void payload_arbiter_try_trigger(payload_arbiter_t *arb,
                                 payload_source_t source)
{
  bool post = false;

  pthread_mutex_lock(&arb->mutex);

  bool allowed =
    (arb->current_mode == PAYLOAD_MODE_MANUAL_SBUS  && source == PAYLOAD_SOURCE_SBUS) ||
    (arb->current_mode == PAYLOAD_MODE_AUTO_MAVLINK && source == PAYLOAD_SOURCE_MAVLINK);

  if (allowed && !arb->executed)
    {
      arb->trigger_requested = true;
      arb->source            = source;
      post = true;
    }

  pthread_mutex_unlock(&arb->mutex);

  if (post)
    {
      sem_post(&arb->sem);
    }
}

void payload_arbiter_rearm(payload_arbiter_t *arb)
{
  pthread_mutex_lock(&arb->mutex);
  arb->trigger_requested = false;
  arb->executed           = false;
  pthread_mutex_unlock(&arb->mutex);
}

int payload_arbiter_executor_entry(payload_arbiter_t *arb)
{
  while (1)
    {
      sem_wait(&arb->sem);

      pthread_mutex_lock(&arb->mutex);
      bool             do_exec = arb->trigger_requested && !arb->executed;
      payload_source_t src     = arb->source;

      if (do_exec)
        {
          arb->executed = true;
        }

      pthread_mutex_unlock(&arb->mutex);

      if (do_exec && arb->on_execute != NULL)
        {
          arb->on_execute(arb->execute_arg, src);
        }
    }

  return 0;
}