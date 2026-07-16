/****************************************************************************
 * common/sbus/sbus_channel_task.c
 *
 * Generic SBUS channel-watching task layer.
 *
 ****************************************************************************/

#include <nuttx/config.h>
#include <errno.h>
#include <debug.h>

#include "sbus_driver.h"
#include "sbus_channel_task.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void process_edge_watch(struct sbus_edge_watch_s *w, uint16_t value)
{
  bool is_high = (value > w->threshold);

  if (!is_high)
    {
      w->high_count = 0;
      w->latched    = false;
      return;
    }

  if (w->high_count < w->debounce_n)
    {
      w->high_count++;
    }

  if (w->high_count >= w->debounce_n && !w->latched)
    {
      w->latched = true;

      if (w->on_edge != NULL)
        {
          w->on_edge(w->cb_arg);
        }
    }
}

static void process_level_watch(struct sbus_level_watch_s *w, uint16_t value)
{
  if (w->on_level != NULL)
    {
      w->on_level(w->cb_arg, value);
    }
}

static void dispatch_frame(struct sbus_channel_task_config_s *cfg,
                           const struct sbus_channels_s *ch)
{
  int i;

  for (i = 0; i < cfg->num_edge_watches; i++)
    {
      struct sbus_edge_watch_s *w = &cfg->edge_watches[i];

      if (w->channel < SBUS_MAX_CHANNELS)
        {
          process_edge_watch(w, ch->channels[w->channel]);
        }
    }

  for (i = 0; i < cfg->num_level_watches; i++)
    {
      struct sbus_level_watch_s *w = &cfg->level_watches[i];

      if (w->channel < SBUS_MAX_CHANNELS)
        {
          process_level_watch(w, ch->channels[w->channel]);
        }
    }
}

static void reset_all_edges(struct sbus_channel_task_config_s *cfg)
{
  int i;

  for (i = 0; i < cfg->num_edge_watches; i++)
    {
      cfg->edge_watches[i].high_count = 0;
      cfg->edge_watches[i].latched    = false;
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int sbus_channel_task_run(struct sbus_channel_task_config_s *cfg)
{
  struct sbus_driver_s   driver;
  struct sbus_channels_s ch;
  bool                    failsafe_active = false;
  int                     eagain_count    = 0;
  int                     ret;

  if (cfg == NULL || cfg->devpath == NULL)
    {
      _err("ERROR: sbus_channel_task: invalid config\n");
      return -EINVAL;
    }

  ret = sbus_driver_init(&driver, cfg->devpath);
  if (ret < 0)
    {
      return ret;
    }

  while (1)
    {
      ret = sbus_driver_read_channels(&driver, &ch);

      if (ret == -EAGAIN)
        {
          if (cfg->failsafe_eagain_limit > 0 &&
              ++eagain_count >= cfg->failsafe_eagain_limit &&
              !failsafe_active)
            {
              failsafe_active = true;
              reset_all_edges(cfg);

              if (cfg->on_failsafe != NULL)
                {
                  cfg->on_failsafe(cfg->failsafe_arg, true);
                }
            }

          continue;
        }

      if (ret < 0)
        {
          /* transient UART error, keep trying */

          continue;
        }

      eagain_count = 0;

      bool frame_failsafe = ch.failsafe || ch.frame_lost;

      if (frame_failsafe != failsafe_active)
        {
          failsafe_active = frame_failsafe;

          if (failsafe_active)
            {
              reset_all_edges(cfg);
            }

          if (cfg->on_failsafe != NULL)
            {
              cfg->on_failsafe(cfg->failsafe_arg, failsafe_active);
            }
        }

      if (failsafe_active)
        {
          continue;
        }

      dispatch_frame(cfg, &ch);
    }

  sbus_driver_deinit(&driver);
  return 0;
}