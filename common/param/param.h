/****************************************************************************
 * common/param/param.h
 *
 * Param Manager - direct Flash Sector 7 access without MTD
 * STM32F411CEU6: Sector 7 @ 0x08060000, 128KB
 *
 ****************************************************************************/

#ifndef __PAYLOAD_CONTROLLER_PARAM_MANAGER_H
#define __PAYLOAD_CONTROLLER_PARAM_MANAGER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/****************************************************************************
 * Defines
 ****************************************************************************/

/* Start address of STM32F411CEU6 sector 7 */
#define PARAM_FLASH_BASE_ADDR       0x08060000UL
#define PARAM_FLASH_SECTOR_SIZE     (128 * 1024)

/* Magic value used to validate stored data */
#define PARAM_MAGIC                 0xDEADBEEFUL

/* Maximum parameter name length according to MAVLink */
#define PARAM_NAME_LEN              16

/* Maximum number of parameters that can be stored */
#define PARAM_MAX_COUNT             32

/****************************************************************************
 * Types
 ****************************************************************************/

/* One parameter: 16 bytes for name + 4 bytes for float = 20 bytes */
struct param_entry_s
{
  char name[PARAM_NAME_LEN];
  float value;
};

struct param_flash_block_s
{
  uint32_t magic;
  uint32_t count;
  uint32_t checksum;
  struct param_entry_s entries[PARAM_MAX_COUNT];
};

/* ---- Enum index ---- */
typedef enum
{
  PARAM_SERVO1_MIN = 0, PARAM_SERVO1_MAX, PARAM_SERVO2_MIN, PARAM_SERVO2_MAX,
  PARAM_SERVO3_MIN, PARAM_SERVO3_MAX, PARAM_SERVO4_MIN, PARAM_SERVO4_MAX,
  PARAM_SERVO5_MIN, PARAM_SERVO5_MAX, PARAM_SERVO6_MIN, PARAM_SERVO6_MAX,
  PARAM_SERVO_NUM, PARAM_PWM_FREQ, PARAM_MAV_SYS_ID, PARAM_MAV_COMP_ID,
} param_id_e;

/* ---- Fast reads ---- */
#define PARAM_GET_FLOAT(id)     (g_params[(id)].value)
#define PARAM_GET_U32(id)       ((uint32_t)g_params[(id)].value)

/****************************************************************************
 * Global Data
 ****************************************************************************/

extern struct param_entry_s g_params[];
extern const int            NUM_PARAMS;

/****************************************************************************
 * API
 ****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

int param_manager_init(void);                           /* Call at boot: load
                                                         * flash or use
                                                         * defaults */
int param_manager_load(void);                           /* Load from flash into
                                                         * g_params[] */
int param_manager_save(void);                           /* Erase sector 7 +
                                                         * write g_params[] to
                                                         * flash */
void param_manager_reset_defaults(void);                /* Reset RAM to
                                                         * defaults without
                                                         * erasing flash */

int param_manager_find(const char *name);               /* Return index or -1 */
int param_manager_get(const char *name, float *value);  /* Get value by name */
int param_manager_set(const char *name, float value);   /* Set RAM only */
void param_manager_print_all(void);                     /* Print all params */

#ifdef __cplusplus
}
#endif

#endif /* __PAYLOAD_CONTROLLER_PARAM_MANAGER_H */
