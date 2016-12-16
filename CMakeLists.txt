cmake_minimum_required (VERSION 2.6)
project (LSM303-TEST)

set(CMAKE_CURRENT_SOURCE_DIR .)

SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_PREFIX /usr/)

add_executable(accelerometer-test accelerometer-test.cpp)
target_link_libraries(accelerometer-test firmware_i2c lsm303)

add_executable(magnetometer-test magnetometer-test.cpp)
target_link_libraries(magnetometer-test firmware_i2c lsm303)

add_executable(calibration calibration.cpp)
target_link_libraries(calibration firmware_i2c lsm303)

install(TARGETS accelerometer-test DESTINATION bin)
install(TARGETS magnetometer-test DESTINATION bin)
install(TARGETS calibration DESTINATION bin)
