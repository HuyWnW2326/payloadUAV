/****************************************************************************
 * common/sbus/sbus_channel_task.h
 *
 * Generic SBUS channel-watching task layer.
 * Sits between sbus_driver (protocol decode) and payload-specific logic.
 * Payload-agnostic: each payload registers its own channel watches and
 * callbacks; this layer only decodes frames and dispatches events.
 *
 ****************************************************************************/

#ifndef __COMMON_SBUS_CHANNEL_TASK_H
#define __COMMON_SBUS_CHANNEL_TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "sbus_driver.h"

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Called once when a monitored channel transitions LOW->HIGH (debounced).
 * Use for momentary switches: arm switches, trigger switches, etc.
 */

typedef void (*sbus_edge_cb_t)(void *arg);

/* Called every valid frame with the raw channel value.
 * Use for continuous/level channels: mode select, analog trims, etc.
 */

typedef void (*sbus_level_cb_t)(void *arg, uint16_t value);

/* Called whenever failsafe/frame-lost condition changes state.
 * `active` is true when signal is considered lost.
 */

typedef void (*sbus_failsafe_cb_t)(void *arg, bool active);

struct sbus_edge_watch_s
{
  uint8_t         channel;
  uint16_t        threshold;
  uint8_t         debounce_n;
  sbus_edge_cb_t  on_edge;
  void           *cb_arg;

  /* internal state, do not set manually */

  uint8_t         high_count;
  bool            latched;
};

struct sbus_level_watch_s
{
  uint8_t          channel;
  sbus_level_cb_t  on_level;
  void            *cb_arg;
};

struct sbus_channel_task_config_s
{
  const char                  *devpath;

  struct sbus_edge_watch_s    *edge_watches;
  int                          num_edge_watches;

  struct sbus_level_watch_s   *level_watches;
  int                          num_level_watches;

  sbus_failsafe_cb_t           on_failsafe;
  void                        *failsafe_arg;

  /* number of consecutive read timeouts (-EAGAIN) before failsafe
   * is declared due to loss of physical signal, independent of the
   * SBUS failsafe flag itself.
   */

  int                          failsafe_eagain_limit;
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* Blocking. Intended to be used directly as a task entry point, e.g.:
 *
 *   static struct sbus_channel_task_config_s g_cfg = { ... };
 *   task_create("sbus_task", prio, stacksize, sbus_channel_task_run, argv);
 *
 * where g_cfg is wired up via a small wrapper entry function, since
 * task_create's entry point cannot take a config pointer directly
 * (see usage note in sbus_channel_task.c).
 */

int sbus_channel_task_run(struct sbus_channel_task_config_s *cfg);

#endif /* __COMMON_SBUS_CHANNEL_TASK_H */