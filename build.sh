#!/bin/bash
TARGET=${1:-drop}
REPO_ROOT=${PWD}

echo ">>> Building payload: $TARGET"

mkdir -p apps/external
cat > apps/external/CMakeLists.txt << CMAKE
add_subdirectory(${REPO_ROOT}/common common_build)
add_subdirectory(${REPO_ROOT}/payloads/${TARGET} payload_build)
CMAKE

if [ ! -f build/$TARGET/build.ninja ]; then
    echo ">>> Configuring..."
    cmake -S nuttx \
          -B build/$TARGET \
          -DBOARD_CONFIG=${REPO_ROOT}/boards/arm/stm32/stm32f411-minimum/configs/$TARGET \
          -DNUTTX_APPS_DIR=${REPO_ROOT}/apps \
          -DREPO_ROOT=${REPO_ROOT} \
          -GNinja
fi

# Build
cmake --build build/$TARGET