/****************************************************************************
 * common/param/param.c
 *
 * Param Manager - store/load parameters directly in Flash Sector 7
 * STM32F411CEU6: Sector 7 @ 0x08060000, 128KB
 *
 * Do not use stm32.h (internal arch header, not accessible from apps).
 * Access Flash registers directly through volatile pointers, which is valid
 * for a NuttX app running on Cortex-M4 without an MMU.
 *
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/irq.h> /* irqstate_t, enter_critical_section(),
                        *            leave_critical_section() */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "param.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Flash register access through volatile pointers instead of getreg32/putreg32
 ****************************************************************************/

#define FLASH_REG_BASE      0x40023C00UL

/* Read/write 32-bit registers through volatile pointers */
#define FLASH_REG(offset)  (*((volatile uint32_t *)(FLASH_REG_BASE + \
                                                    (offset))))

#define FLASH_KEYR          FLASH_REG(0x04)
#define FLASH_SR            FLASH_REG(0x0C)
#define FLASH_CR            FLASH_REG(0x10)

/* FLASH_CR bits */
#define FLASH_CR_PG         (1U << 0)
#define FLASH_CR_SER        (1U << 1)
#define FLASH_CR_SNB_SHIFT  3
#define FLASH_CR_SNB_MASK   (0xFU << FLASH_CR_SNB_SHIFT)
#define FLASH_CR_PSIZE_X32  (0x2U << 8)
#define FLASH_CR_STRT       (1U << 16)
#define FLASH_CR_LOCK       (1U << 31)

/* FLASH_SR bits */
#define FLASH_SR_BSY        (1U << 16)
#define FLASH_SR_ERR_MASK   (0xF2U)

/* Unlock keys */
#define FLASH_KEY1          0x45670123UL
#define FLASH_KEY2          0xCDEF89ABUL

/****************************************************************************
 * Param table - add/remove parameters here.
 * Name is limited to 15 characters plus NUL. Value is the default when flash
 * is empty.
 ****************************************************************************/

struct param_entry_s    g_params[] = {
  { "SERVO1_MIN", 1500.0f          }, { "SERVO1_MAX",  2000.0f            },
  { "SERVO2_MIN", 1000.0f          }, { "SERVO2_MAX",  2000.0f            },
  { "SERVO3_MIN", 1000.0f          }, { "SERVO3_MAX",  2000.0f            },
  { "SERVO4_MIN", 1000.0f          }, { "SERVO4_MAX",  2000.0f            },
  { "SERVO5_MIN", 1000.0f          }, { "SERVO5_MAX",  2000.0f            },
  { "SERVO6_MIN", 1000.0f          }, { "SERVO6_MAX",  2000.0f            },
  { "SERVO_NUM",  6.0f             }, { "PWM_FREQ",    50.0f              },
  { "MAV_SYS_ID", 1.0f             }, { "MAV_COMP_ID", 236.0f           },
};

const int NUM_PARAMS = (int)(sizeof(g_params) / sizeof(g_params[0]));


/* Flash read pointer (memory-mapped, read-only) */
static const struct param_flash_block_s *const FLASH_PARAM = 
  (const struct param_flash_block_s *)PARAM_FLASH_BASE_ADDR;

/* Default backup for reset */
static float    g_defaults[PARAM_MAX_COUNT];
static bool     g_defaults_saved = false;

/****************************************************************************
 * Flash low-level
 ****************************************************************************/

static int flash_wait_done(void)
{
  volatile uint32_t timeout = 100000000UL;

  while (FLASH_SR & FLASH_SR_BSY)
    {
      if (--timeout == 0)
        {
          printf("[PARAM] ERROR: Flash busy timeout\n");
          return -ETIMEDOUT;
        }
    }

  uint32_t sr = FLASH_SR;

  if (sr & FLASH_SR_ERR_MASK)
    {
      printf("[PARAM] ERROR: Flash SR=0x%08lX\n", (unsigned long)sr);
      FLASH_SR = sr & FLASH_SR_ERR_MASK; /* clear error bits (rc_w1) */
      return -EIO;
    }

  return 0;
}

static void flash_unlock(void)
{
  if (!(FLASH_CR & FLASH_CR_LOCK))
    {
      return;
    }
  FLASH_KEYR    = FLASH_KEY1;
  FLASH_KEYR    = FLASH_KEY2;
}

static void flash_lock(void)
{
  FLASH_CR |= FLASH_CR_LOCK;
}

static int flash_erase_sector7(void)
{
  int ret = flash_wait_done();

  if (ret < 0)
    {
      return ret;
    }

  /* SER=1, SNB=7, PSIZE=x32 */
  uint32_t cr = FLASH_CR;

  cr        &= ~FLASH_CR_SNB_MASK;
  cr        |= FLASH_CR_SER | FLASH_CR_PSIZE_X32 | (7U << FLASH_CR_SNB_SHIFT);
  FLASH_CR  = cr;
  FLASH_CR  |= FLASH_CR_STRT;

  ret = flash_wait_done();

  /* Clear SER */
  FLASH_CR &= ~(FLASH_CR_SER | FLASH_CR_SNB_MASK);
  return ret;
}

static int flash_write_word(uint32_t addr, uint32_t data)
{
  int ret = flash_wait_done();

  if (ret < 0)
    {
      return ret;
    }

  uint32_t cr = FLASH_CR;

  cr        = (cr & ~FLASH_CR_PSIZE_X32) | FLASH_CR_PG | FLASH_CR_PSIZE_X32;
  FLASH_CR  = cr;

  *((volatile uint32_t *)addr) = data;

  ret       = flash_wait_done();
  FLASH_CR  &= ~FLASH_CR_PG;
  return ret;
}

static int flash_write_buffer(uint32_t addr, const void *buf, size_t size)
{
  const uint32_t *src = (const uint32_t *)buf;

  for (size_t i = 0; i < size / 4; i++)
    {
      int ret = flash_write_word(addr + i * 4, src[i]);
      if (ret < 0)
        {
          printf("[PARAM] ERROR: Write failed at word %zu: %d\n", i, ret);
          return ret;
        }
    }
  return 0;
}

/****************************************************************************
 * Checksum & defaults
 ****************************************************************************/

static uint32_t calc_checksum(const struct param_entry_s *entries, int count)
{
  uint32_t csum = 0;

  for (int i = 0; i < count; i++)
    {
      uint32_t raw;
      memcpy(&raw, &entries[i].value, sizeof(float));
      csum ^= raw;
    }
  return csum;
}

static void save_defaults(void)
{
  if (g_defaults_saved)
    {
      return;
    }
  int n = NUM_PARAMS < PARAM_MAX_COUNT ? NUM_PARAMS : PARAM_MAX_COUNT;

  for (int i = 0; i < n; i++)
    {
      g_defaults[i] = g_params[i].value;
    }
  g_defaults_saved = true;
}

/****************************************************************************
 * Public API
 ****************************************************************************/

int param_manager_init(void)
{
    save_defaults();
    printf("[PARAM] Init @ 0x%08lX (sector 7)\n",
           (unsigned long)PARAM_FLASH_BASE_ADDR);

    int ret = param_manager_load();
    if (ret != 0)
    {
        printf("[PARAM] Using factory defaults\n");
        param_manager_reset_defaults();
    }

    param_manager_print_all();
    return 0;
}

int param_manager_load(void)
{
  const struct param_flash_block_s *blk = FLASH_PARAM;

  if (blk->magic != PARAM_MAGIC)
    {
      printf("[PARAM] Load: magic=0x%08lX invalid -> use defaults\n",
             (unsigned long)blk->magic);
      return 1;
    }

  if (blk->count == 0 || blk->count > (uint32_t)PARAM_MAX_COUNT)
    {
      printf("[PARAM] Load: bad count %lu -> use defaults\n",
             (unsigned long)blk->count);
      return 1;
    }

  uint32_t csum = calc_checksum(blk->entries, (int)blk->count);

  if (csum != blk->checksum)
    {
      printf("[PARAM] Load: checksum FAIL (calc=0x%08lX stored=0x%08lX)\n",
             (unsigned long)csum, (unsigned long)blk->checksum);
      return -EBADMSG;
    }

  int matched = 0;

  for (uint32_t fi = 0; fi < blk->count; fi++)
    {
      for (int gi = 0; gi < NUM_PARAMS; gi++)
        {
          if (strncmp(g_params[gi].name, blk->entries[fi].name,
                      PARAM_NAME_LEN)
              == 0)
            {
              g_params[gi].value = blk->entries[fi].value;
              matched++;
              break;
            }
        }
    }

  printf("[PARAM] Load OK: %d/%lu params restored\n", matched,
         (unsigned long)blk->count);
  return 0;
}

void param_manager_reset_defaults(void)
{
  if (!g_defaults_saved)
    {
      return;
    }
  int n = NUM_PARAMS < PARAM_MAX_COUNT ? NUM_PARAMS : PARAM_MAX_COUNT;

  for (int i = 0; i < n; i++)
    {
      g_params[i].value = g_defaults[i];
    }
  printf("[PARAM] Reset to defaults\n");
}

int param_manager_save(void)
{
  _Static_assert(sizeof(struct param_flash_block_s) % 4 == 0,
                 "param_flash_block_s size must be multiple of 4");

  if (NUM_PARAMS > PARAM_MAX_COUNT)
    {
      printf("[PARAM] ERROR: too many params (%d > %d)\n", NUM_PARAMS,
             PARAM_MAX_COUNT);
      return -EINVAL;
    }

  static struct param_flash_block_s block;

  memset(&block, 0xFF, sizeof(block));

  block.magic   = PARAM_MAGIC;
  block.count   = (uint32_t)NUM_PARAMS;

  for (int i = 0; i < NUM_PARAMS; i++)
    {
      strncpy(block.entries[i].name, g_params[i].name, PARAM_NAME_LEN - 1);
      block.entries[i].name[PARAM_NAME_LEN - 1] = '\0';
      block.entries[i].value                    = g_params[i].value;
    }

  block.checksum = calc_checksum(block.entries, NUM_PARAMS);

  printf("[PARAM] Saving %d params to 0x%08lX...\n", NUM_PARAMS,
         (unsigned long)PARAM_FLASH_BASE_ADDR);

  irqstate_t flags = enter_critical_section();

  flash_unlock();
  int ret = flash_erase_sector7();

  if (ret == 0)
    {
      ret = flash_write_buffer(PARAM_FLASH_BASE_ADDR, &block, sizeof(block));
    }
  flash_lock();

  leave_critical_section(flags);

  if (ret < 0)
    {
      printf("[PARAM] ERROR: Save failed: %d\n", ret);
      return ret;
    }

  /* Verify through the memory-mapped pointer */
  if (FLASH_PARAM->magic != block.magic ||
      FLASH_PARAM->count != block.count ||
      FLASH_PARAM->checksum != block.checksum)
    {
      printf("[PARAM] ERROR: Verify FAILED!\n");
      return -EIO;
    }

  printf("[PARAM] Save OK  magic=0x%08lX count=%lu csum=0x%08lX\n",
         (unsigned long)block.magic, (unsigned long)block.count,
         (unsigned long)block.checksum);
  return 0;
}

int param_manager_find(const char *name)
{
  if (!name)
    {
      return -1;
    }
  for (int i = 0; i < NUM_PARAMS; i++)
    {
      if (strncmp(g_params[i].name, name, PARAM_NAME_LEN) == 0)
        {
          return i;
        }
    }
  return -1;
}

int param_manager_get(const char *name, float *value)
{
  int idx = param_manager_find(name);

  if (idx < 0)
    {
      return -1;
    }
  if (value)
    {
      *value = g_params[idx].value;
    }
  return 0;
}

int param_manager_set(const char *name, float value)
{
  int idx = param_manager_find(name);

  if (idx < 0)
    {
      return -1;
    }
  printf("[PARAM] SET %s: %.3f -> %.3f\n", g_params[idx].name,
         (double)g_params[idx].value, (double)value);
  g_params[idx].value = value;
  return 0;
}

void param_manager_print_all(void)
{
  printf("[PARAM] ===== %d params =====\n", NUM_PARAMS);
  for (int i = 0; i < NUM_PARAMS; i++)
    {
      printf("[PARAM]   [%2d] %-16s = %.3f\n", i, g_params[i].name,
             (double)g_params[i].value);
    }
  printf("[PARAM] ======================\n");
}
