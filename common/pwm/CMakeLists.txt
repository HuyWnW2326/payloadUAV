nuttx_add_library(pwm STATIC
  pwm_driver.c
)

target_include_directories(pwm PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${REPO_ROOT}/common
)
