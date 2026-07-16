if(EXISTS ${NUTTX_APPS_BINDIR}/Kconfig)
  add_library(payload_arbiter STATIC
    payload_arbiter.c
  )
  nuttx_add_external_library(payload_arbiter)
  target_include_directories(payload_arbiter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${REPO_ROOT}/common
  )
endif()
