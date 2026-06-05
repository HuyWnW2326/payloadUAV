nuttx_add_library(param STATIC
  param.c
)

target_include_directories(param PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${REPO_ROOT}/common
)
