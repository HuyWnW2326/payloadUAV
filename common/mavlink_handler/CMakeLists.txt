if(EXISTS ${NUTTX_APPS_BINDIR}/Kconfig)
  add_library(mavlink_handler STATIC
    mavlink_handler.c
  )
  nuttx_add_external_library(mavlink_handler)
  target_include_directories(mavlink_handler PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${REPO_ROOT}/common
  )
endif()
