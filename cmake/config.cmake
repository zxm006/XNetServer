# Cross build?
option (CROSS_BUILD "Build cross platform" OFF)

# Should we build release mode?
option (RELEASE_MODE "Build release mode" OFF)

configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/config.h.in"
  "${PROJECT_BINARY_DIR}/cmake/config.h"
)
