if(EXISTS ${NUTTX_APPS_BINDIR}/Kconfig)
  add_library(sbus STATIC
    sbus_driver.c
    sbus_channel_task.c
  )
  nuttx_add_external_library(sbus)
  target_include_directories(sbus PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${REPO_ROOT}/common
  )
endif()
