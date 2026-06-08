if(EXISTS ${NUTTX_APPS_BINDIR}/Kconfig)
  add_library(param STATIC
    param.c
  )
  nuttx_add_external_library(param)
  target_include_directories(param PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${REPO_ROOT}/common
  )
endif()
