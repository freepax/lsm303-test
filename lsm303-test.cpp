#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <bmp180.h>


static const int samples = 7;
unsigned char oss_mode = ModeOss3;


int main(int argc, char **argv)
{
    long pressure[samples];
    float temperature = 0.0;

    /// create bmp180 instance - set device to "/dev/i2c-1"
    Bmp180 bmp180((char*)FirmwareI2CDeviceses::i2c_1);

    /// open device (this will also read calibration data)
    if (bmp180.openDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " openDevice failed" << std::endl;
        return -1;
    }

    ///  read chip ID (should be 0x55)
    int id = bmp180.readChipId();
    if (id < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " readChipId failed" << std::endl;
        return -2;
    }

    std::cout << "Chip ID 0x" << std::hex << id << std::dec << std::endl;

    /// temperature...
    if (bmp180.readTemperature(&temperature) < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " readTemperature failed" << std::endl;
        return -1;
    }

    std::cout << "temperature " << std::dec << temperature << std::endl;


    /// read temperature and pressure 10 times
    while (1) {
        if (bmp180.readPressure(pressure, oss_mode, samples, true) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " readPressure failed" << std::endl;
            return 0;
        }

        /// pressure...
        if (bmp180.readPressure(pressure, oss_mode, samples, true) < 0) {
            std::cerr << __func__ << ":" << __LINE__ << " readPressure failed" << std::endl;
            return 0;
        }

        long pa = 0;
        for (int i = 0; i < samples; i++)
            pa += pressure[i];
        pa /= samples;


        std::cout << "Pressure " << std::dec << pa;
        //printf("Temperaur %4.2f Pressure %ld    Altitude %4.2f\n", temperature, pa, altitude(pa, 99700));
    }

    if (bmp180.closeDevice() < 0) {
        std::cerr << __func__ << ":" << __LINE__ << " closeDevice failed" << std::endl;
        return 0;
    }

    return 0;
}
