# Install script for directory: /home/huy/CTUAV/payloadUAV/apps/examples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "arm-none-eabi-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/abntcodi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/adjtime/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ads7046/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/adxl372_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ajoystick/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/alarm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/amg88xx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/apa102/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/apds9960/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/audio_rttl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/bastest/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/battery/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ble/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/bme680/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/bmi160/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/bmp180/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/bmp280/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/bridge/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/buttons/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/calib_udelay/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/camera/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/can/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/capture/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/cbortest/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/cctype/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/charger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/chat/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/chrono/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/configdata/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/cordic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/cpuhog/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/cromfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/dac/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/dhcpd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/dhtxx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/discover/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/djoystick/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/dronecan/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/elf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/embedlog/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/esp32_himem/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/etl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/fb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/fbcon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/fboverlay/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/flash_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/flowc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/fmsynth/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/foc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ft80x/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ftpc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ftpd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/fxos8700cq_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/gps/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/hall/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/hdc1008_demo/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/hello/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/hello_wasm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/helloxx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/hidkbd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/hts221_reader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/hx711/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/i2schar/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/i2sloop/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/igmp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ina219/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ina226/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ini_dumper/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ipcfg/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ipforward/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/isl29023/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/json/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/leds/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/libtest/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/lis3dsh_reader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/lp503x/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/lsm303_reader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/lsm330spi_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/lsm6dsl_reader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ltr308/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/lua_module/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/lvgldemo/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/lvglterm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/max31855/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/mcuboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/media/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/microros_pub/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/mld/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/mlx90614/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/mml_parser/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/modbus/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/modbusmaster/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/module/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/mount/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/mqttc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/mtdpart/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/mtdrwb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/netlink_route/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/netloop/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/netpkt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nettest/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nimble/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nimble_blecent/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nimble_bleprph/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nng_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/noteprintf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nrf24l01_btle/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nrf24l01_term/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/null/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nunchuck/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nxdemo/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nxflat/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nxhello/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nximage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nxlines/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nxmbserver/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nxscope/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nxterm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/nxtext/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/obd2/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/oneshot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/opencyphal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/optee/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/optee_gp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/pca9635/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/pdcurses/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/pf_ieee802154/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/pipe/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/poll/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/popen/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/posix_spawn/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/posix_stdio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/powerled/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/powermonitor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/pppd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/pty_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/pulsecount/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/pwfb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/pwlines/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/pwm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/qencoder/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/random/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/relays/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/rfid_readuid/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/rgbled/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/romfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/rpmsgsocket/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/rust/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/scd41/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/sendmail/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/serialblaster/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/serialrx/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/serloop/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/shm_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/sht3x/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/slcd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/smf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/smps/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/sotest/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/spislv_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/stat/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/stepper/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/sx127x_demo/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/system/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/tcp_ipc_client/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/tcp_ipc_server/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/tcpblaster/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/tcpecho/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/telnetd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/termios/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/thttpd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/tiff/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/timer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/timer_gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/tmp112/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/touchscreen/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/udgram/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/udp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/udpblaster/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/uid/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/unionfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/usbserial/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/userfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/usrsocktest/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ustream/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/uvc_cam/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/veml6070/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/wamr_module/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/watchdog/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/watched/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/watcher/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/webserver/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/wget/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/wgetjson/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ws2812/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/ws2812esp32rmt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/xbc_test/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/xmlrpc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/examples/zerocross/cmake_install.cmake")
endif()

