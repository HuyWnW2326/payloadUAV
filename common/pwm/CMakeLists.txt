if(EXISTS ${NUTTX_APPS_BINDIR}/Kconfig)
  add_library(pwm STATIC
    pwm_driver.c
  )
  nuttx_add_external_library(pwm)
  target_include_directories(pwm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${REPO_ROOT}/common
  )
endif()
