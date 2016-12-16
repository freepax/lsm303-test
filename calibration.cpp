#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <lsm303Accelerometer.h>
#include <lsm303Magnetometer.h>


#define ARG_HELP        'h'
#define ARG_VERBOSE     'd'
#define ARGLIST         "hd"

int verbose = 0;


int getoptarg(int argc, char **argv)
{

    int opt;

    do {
        opt = getopt(argc, argv, ARGLIST);
        if (opt < 0)
            break;

        switch (opt) {
        case ARG_HELP:
            printf("print help\n");
            exit(EXIT_SUCCESS);
            break;

        case ARG_VERBOSE:
            verbose = 1;
            break;

        default:
            printf("default print help\n");
            exit(EXIT_FAILURE);
        }
    } while (opt != -1);

    return 0;
}


int main(int argc, char **argv)
{
    accel_event_t accel_event;
    magnetic_event_t mag_event;

    int status = getoptarg(argc, argv);
    if (status < 0) {
        std::cerr << __func__ << ": getoptarg failed with error " << status << std::endl;
        return 0;
    }

    /// create lsm303accelerometer instance - set device to "/dev/i2c-1"
    LSM303Accelerometer lsm303accelerometer((char*)FirmwareI2CDeviceses::i2c_1);

    /// open the accelerometer device
    if (lsm303accelerometer.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return -1;
    }

    ///  initialize chip
    int accel_id = lsm303accelerometer.init();
    if (accel_id < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " readChipId failed" << std::endl;
        return -2;
    }

    /// create lsm303magnetometer instance - set device to "/dev/i2c-1"
    LSM303Magnetometer lsm303magnetometer((char*)FirmwareI2CDeviceses::i2c_1);

    /// open device
    if (lsm303magnetometer.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return -3;
    }

    ///  initialize chip
    int mag_id = lsm303magnetometer.init();
    if (mag_id < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " readChipId failed" << std::endl;
        return -4;
    }

    float acc_xmax, acc_ymax, acc_zmax;
    float acc_xmin, acc_ymin, acc_zmin;
    int mag_xmax, mag_ymax, mag_zmax;
    int mag_xmin, mag_ymin, mag_zmin;

    acc_xmax = acc_ymax = acc_zmax = -4294967295.0f;
    acc_xmin = acc_ymin = acc_zmin = 4294967295.0f;
    mag_xmax = mag_ymax = mag_zmax = -65536;
    mag_xmin = mag_ymin = mag_zmin = 65536;

    while (1) {
        /// read accelerometer
        if (lsm303accelerometer.getEvent(&accel_event) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " getEvent failed" << std::endl;
            continue;
        }

        /// acceleration max x and min x
        if (accel_event.acceleration.x > acc_xmax)
            acc_xmax = accel_event.acceleration.x;

        if (accel_event.acceleration.x < acc_xmin)
            acc_xmin = accel_event.acceleration.x;

        /// acceleration max y and min y
        if (accel_event.acceleration.y > acc_ymax)
            acc_ymax = accel_event.acceleration.y;

        if (accel_event.acceleration.y < acc_ymin)
            acc_ymin = accel_event.acceleration.y;

        /// acceleration max z and min z
        if (accel_event.acceleration.z > acc_zmax)
            acc_zmax = accel_event.acceleration.z;

        if (accel_event.acceleration.z < acc_zmin)
            acc_zmin = accel_event.acceleration.z;

        /// read magnetometer
        if (lsm303magnetometer.getEvent(&mag_event) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " getEvent failed with error " << status << std::endl;
            continue;
        }

        /// magnetometer max x and min x
        if (mag_event.magnetic.x > mag_xmax)
            mag_xmax = mag_event.magnetic.x;

        if (mag_event.magnetic.x < mag_xmin)
            mag_xmin = mag_event.magnetic.x;

        /// magnetometer max y and min y
        if (mag_event.magnetic.y > mag_ymax)
            mag_ymax = mag_event.magnetic.y;

        if (mag_event.magnetic.y < mag_ymin)
            mag_ymin = mag_event.magnetic.y;

        /// magnetometer max z and min z
        if (mag_event.magnetic.z > mag_zmax)
            mag_zmax = mag_event.magnetic.z;

        if (mag_event.magnetic.z < mag_zmin)
            mag_zmin = mag_event.magnetic.z;

        if (verbose) {
            printf("Source data Accelerometer: %4.4f %4.4f %4.4f\n", accel_event.acceleration.x, accel_event.acceleration.y, accel_event.acceleration.z);
            printf("Source data Magnetometer : %04d  %04d  %04d\n", mag_event.magnetic.x, mag_event.magnetic.y, mag_event.magnetic.z);
        }

        printf("accelerometer xminmax %2.4f %2.4f yminmax %2.4f %2.4f zminmax %2.4f %2.4f\n", acc_xmin, acc_xmax, acc_ymin, acc_ymax, acc_zmin, acc_zmax);
        printf("magnetometer  xminmax %6d %6d yminmax %6d %6d zminmax %6d %6d\n", mag_xmin, mag_xmax, mag_ymin, mag_ymax, mag_zmin, mag_zmax);

        usleep(500000);
    }

    return 0;
}

