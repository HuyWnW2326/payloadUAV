#!/bin/bash
TARGET=${1:-drop}
BIN=build/$TARGET/nuttx.bin

echo ">>> Flashing payload: $TARGET"

if [ ! -f "$BIN" ]; then
    echo ">>> ERROR: $BIN not found. Build it first with: ./build.sh $TARGET"
    exit 1
fi

openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
  -c "transport select swd" \
  -c "adapter speed 100" \
  -c "reset_config srst_only srst_nogate connect_assert_srst" \
  -c "init" \
  -c "reset halt" \
  -c "flash write_image erase $BIN 0x08000000" \
  -c "reset run" \
  -c "shutdown"