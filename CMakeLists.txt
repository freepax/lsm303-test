cmake_minimum_required (VERSION 2.6)
project (LSM303-TEST)

set(CMAKE_CURRENT_SOURCE_DIR .)

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_PREFIX /usr/)

set(LSM303_SOURCE
  lsm303-test.cpp
)

add_executable(lsm303-test ${LSM303_SOURCE})
target_link_libraries(lsm303-test firmware_i2c lsm303lib)

install(TARGETS lsm303-test DESTINATION bin)
