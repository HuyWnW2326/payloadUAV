nuttx_add_library(mavlink_handler STATIC
  mavlink_handler.c
)

target_include_directories(mavlink_handler PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${REPO_ROOT}/common
)
