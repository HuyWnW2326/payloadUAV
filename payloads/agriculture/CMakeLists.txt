nuttx_add_application(
  NAME agriculture_payload
  SRCS agricultural_main.c
  INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${REPO_ROOT}/common
)
