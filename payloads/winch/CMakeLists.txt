nuttx_add_application(
  NAME winch_payload
  SRCS winch_main.c
  INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${REPO_ROOT}/common
)
