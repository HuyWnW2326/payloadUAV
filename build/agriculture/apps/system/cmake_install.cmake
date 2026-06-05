# Install script for directory: /home/huy/CTUAV/payloadUAV/apps/system

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
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/adb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/adcscope/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/argtable3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/batterydump/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/cdcacm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/cfgdata/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/cle/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/composite/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/conntrack/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/coredump/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/cpuload/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/critmon/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/cu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/dd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/debugpoint/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/dhcp6c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/dhcpc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/dumpstack/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/fastboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/fdt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/flash_eraseall/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/flatbuffers/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/gcov/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/gdbstub/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/gprof/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/hex2bin/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/hexed/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/hostname/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/i2c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/iptables/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/irtest/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/libuv/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/lm75/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/lsan/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/lzf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/mdio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/microros/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/netdb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/nsh/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/ntpc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/nxcamera/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/nxcodec/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/nxdiag/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/nxinit/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/nxlooper/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/nxmbclient/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/nxplayer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/nxrecorder/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/ofloader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/ping/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/ping6/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/popen/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/ptpd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/readline/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/resmonitor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/sched_note/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/sensorscope/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/sensortest/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/setlogmask/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/spi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/stackmonitor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/syslogd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/system/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/taskset/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/tcpdump/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/tee/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/telnet/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/telnetd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/termcurses/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/trace/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/ubloxmodem/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/uniqueid/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/uorb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/usbmsc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/vi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/vncviewer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/ymodem/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/zlib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/huy/CTUAV/payloadUAV/build/agriculture/apps/system/zmodem/cmake_install.cmake")
endif()

