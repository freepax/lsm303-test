#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <lsm303lib.h>




int main(int argc, char **argv)
{
    sensors_event_t sensor_event;

    /// create lsm303accelerometer instance - set device to "/dev/i2c-1"
    LSM303Accelerometer lsm303accelerometer((char*)FirmwareI2CDeviceses::i2c_1);

    /// open device (this will also read calibration data)
    if (lsm303accelerometer.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return -1;
    }

    ///  initialize chip
    int id = lsm303accelerometer.init();
    if (id < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " readChipId failed" << std::endl;
        return -2;
    }

    /// read temperature and pressure 10 times
    while (1) {
        /// get acceleration data
        if (lsm303accelerometer.getEvent(&sensor_event) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " readTemperature failed" << std::endl;
            return -1;
        }

        std::cout << "acceleration x " << std::dec << sensor_event.acceleration.x << " acceleration y " << sensor_event.acceleration.y << " acceleration z " << sensor_event.acceleration.z << std::endl;

        sleep(1);
    }

    if (lsm303accelerometer.closeDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " closeDevice failed" << std::endl;
        return 0;
    }

    return 0;
}
