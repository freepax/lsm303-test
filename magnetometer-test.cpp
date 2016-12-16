#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <lsm303Magnetometer.h>

#define AVARAGE 10


int main(int argc, char **argv)
{
    magnetic_event_t sensor_event;
//    sensor_t sensor;

    /// create lsm303magnetometer instance - set device to "/dev/i2c-1"
    LSM303Magnetometer lsm303magnetometer((char*)FirmwareI2CDeviceses::i2c_1);

    /// open device (this will also read calibration data)
    if (lsm303magnetometer.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return -1;
    }
#if 0
    /// get sensor information from library
    lsm303magnetometer.getSensor(&sensor);
    printf("Sensor name         %s\n", sensor.name);
    printf("Sensor version      0x%0x\n", sensor.version);
    printf("Sensor id           0x%0x\n", sensor.sensor_id);
    printf("Sensor type         %d\n", sensor.type);

    printf("Sensor max value    %4.2f\n", sensor.max_value);
    printf("Sensor min value    %4.2f\n", sensor.min_value);
    printf("Sensor type         %4.2f\n", sensor.resolution);
    printf("Sensor min delay    %d\n", sensor.min_delay);
#endif

#if 0
    int status = readRegisters(lsm303magnetometer);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " readRegisters failed with status " << status << std::endl;
        return 0;
    }
#endif

    ///  initialize chip
    int id = lsm303magnetometer.init();
    if (id < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " readChipId failed" << std::endl;
        return -2;
    }

    int i;
    int x;
    int y;
    int z;

    /// read magnetometer
    while (1) {
        top:
        x = 0;
        y = 0;
        z = 0;

        for (i = 0; i < AVARAGE; i++) {
            /// get magnetic data
            int status = lsm303magnetometer.getEvent(&sensor_event);
            if (status < 0) {
                std::cerr << __func__ << ":" << __LINE__ << " getEvent failed with error " << status << std::endl;
                goto top;
            }
            x += sensor_event.magnetic.x;
            y += sensor_event.magnetic.y;
            z += sensor_event.magnetic.z;
        }

        x /= AVARAGE;
        y /= AVARAGE;
        z /= AVARAGE;

        //std::cout << "magnetic x " << std::dec << x << " magnetic y " << y << " magnetic z " << z << std::endl;
        printf("%s: x    %04d    y    %04d    z    %04d\n", __func__, x, y, z);
    }

    if (lsm303magnetometer.closeDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " closeDevice failed" << std::endl;
        return 0;
    }

    return 0;
}
