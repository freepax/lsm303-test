#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <lsm303Accelerometer.h>

#define AVARAGE 10

int readRegisters(LSM303Accelerometer lsm303accelerometer)
{
    int status = -1;
    char reg = -1;
    char value = -1;

    reg = LSM303RegisterAccelerometerCtrlReg1_A;
    value = -1;

    status = lsm303accelerometer.getRegister(reg, &value);
    if (status < 0) {
        std::cerr << __func__ << " getRegister failed with erro " << status << std::endl;
        return -1;
    }
    printf("LSM303RegisterAccelerometerCtrlReg1_A 0x%0x : 0x%0x\n", reg, value);

    reg = LSM303RegisterAccelerometerCtrlReg2_A;
    value = -1;
    status = lsm303accelerometer.getRegister(reg, &value);
    if (status < 0) {
        std::cerr << __func__ << " getRegister failed with erro " << status << std::endl;
        return -1;
    }
    printf("LSM303RegisterAccelerometerCtrlReg2_A 0x%0x : 0x%0x\n", reg, value);

    reg = LSM303RegisterAccelerometerCtrlReg3_A;
    value = -1;
    status = lsm303accelerometer.getRegister(reg, &value);
    if (status < 0) {
        std::cerr << __func__ << " getRegister failed with erro " << status << std::endl;
        return -1;
    }
    printf("LSM303RegisterAccelerometerCtrlReg3_A 0x%0x : 0x%0x\n", reg, value);

    reg = LSM303RegisterAccelerometerCtrlReg4_A;
    value = -1;
    status = lsm303accelerometer.getRegister(reg, &value);
    if (status < 0) {
        std::cerr << __func__ << " getRegister failed with erro " << status << std::endl;
        return -1;
    }
    printf("LSM303RegisterAccelerometerCtrlReg4_A 0x%0x : 0x%0x\n", reg, value);

    reg = LSM303RegisterAccelerometerCtrlReg5_A;
    value = -1;
    status = lsm303accelerometer.getRegister(reg, &value);
    if (status < 0) {
        std::cerr << __func__ << " getRegister failed with erro " << status << std::endl;
        return -1;
    }
    printf("LSM303RegisterAccelerometerCtrlReg5_A 0x%0x : 0x%0x\n", reg, value);

    reg = LSM303RegisterAccelerometerCtrlReg6_A;
    value = -1;
    status = lsm303accelerometer.getRegister(reg, &value);
    if (status < 0) {
        std::cerr << __func__ << " getRegister failed with erro " << status << std::endl;
        return -1;
    }
    printf("LSM303RegisterAccelerometerCtrlReg6_A 0x%0x : 0x%0x\n", reg, value);

    reg = LSM303RegisterAccelerometerReference_A;
    value = -1;
    status = lsm303accelerometer.getRegister(reg, &value);
    if (status < 0) {
        std::cerr << __func__ << " getRegister failed with erro " << status << std::endl;
        return -1;
    }
    printf("LSM303RegisterAccelerometerReference_A 0x%0x : 0x%0x\n", reg, value);

    reg = LSM303RegisterAccelerometerStatusReg_A;
    value = -1;
    status = lsm303accelerometer.getRegister(reg, &value);
    if (status < 0) {
        std::cerr << __func__ << " getRegister failed with erro " << status << std::endl;
        return -1;
    }
    printf("LSM303RegisterAccelerometerStatusReg_A 0x%0x : 0x%0x\n", reg, value);

    return 0;
}


int main(int argc, char **argv)
{
    sensor_event_t sensor_event;
//    sensor_t sensor;

    /// create lsm303accelerometer instance - set device to "/dev/i2c-1"
    LSM303Accelerometer lsm303accelerometer((char*)FirmwareI2CDeviceses::i2c_1);

    /// open device (this will also read calibration data)
    if (lsm303accelerometer.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return -1;
    }
#if 0
    /// get sensor information from library
    lsm303accelerometer.getSensor(&sensor);
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
    int status = readRegisters(lsm303accelerometer);
    if (status < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " readRegisters failed with status " << status << std::endl;
        return 0;
    }
#endif

    ///  initialize chip
    int id = lsm303accelerometer.init();
    if (id < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " readChipId failed" << std::endl;
        return -2;
    }

    int i;
    float x;
    float y;
    float z;

    /// read accelerometer
    while (1) {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;

        for (i = 0; i < AVARAGE; i++) {
            /// get acceleration data
            if (lsm303accelerometer.getEvent(&sensor_event) < 0) {
                std::cerr << __func__ << ":" << __LINE__ << " getEvent failed" << std::endl;
                return -3;
            }
            x += sensor_event.acceleration.x;
            y += sensor_event.acceleration.y;
            z += sensor_event.acceleration.z;
        }

        x /= (float)AVARAGE;
        y /= (float)AVARAGE;
        z /= (float)AVARAGE;

        //std::cout << "acceleration x " << std::dec << x << " acceleration y " << y << " acceleration z " << z << std::endl;
        printf("x    %4.04f    y    %4.04f    z    %4.04f\n", x, y, z);
    }

    if (lsm303accelerometer.closeDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " closeDevice failed" << std::endl;
        return 0;
    }

    return 0;
}
