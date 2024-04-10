#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <math.h>

#define GYRO_DEVICE_ADDR 0x68
#define PWR_MGMT_1 0x6B
#define GYRO_CONFIG 0x1B
#define GYRO_XOUT_H 0x43

static int fd = -1;
static float yaw = 0.0;

void Gyroscope_init(void)
{
    char *bus = "/dev/i2c-2";
    if ((fd = open(bus, O_RDWR)) < 0)
    {
        perror("Failed to open the bus");
        exit(1);
    }
    if (ioctl(fd, I2C_SLAVE, GYRO_DEVICE_ADDR) < 0)
    {
        perror("Failed to acquire bus access and/or talk to slave.");
        exit(1);
    }

    char config[2] = {0};
    config[0] = PWR_MGMT_1;
    config[1] = 0x00; // Wake up the sensor
    write(fd, config, 2);

    config[0] = GYRO_CONFIG;
    config[1] = 0x18; // ±2000 deg/sec
    write(fd, config, 2);

    yaw = 0.0; // Reset yaw
}

void Gyroscope_cleanUp(void)
{
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}

int Gyroscope_getAngle(void)
{
    // Simplified version of getting gyro data and calculating angle
    char reg[1] = {GYRO_XOUT_H};
    char data[6];
    if (write(fd, reg, 1) != 1 || read(fd, data, 6) != 6)
    {
        perror("Failed to read from the device");
        return 0;
    }

    int16_t gyro_x = (data[0] << 8) | data[1];
    int16_t gyro_y = (data[2] << 8) | data[3];
    int16_t gyro_z = (data[4] << 8) | data[5];

    // Very simplified - Just returning the z-axis rotation as "angle" for demonstration
    return gyro_z;
}

int Gyroscope_getDirection(void)
{
    // Assuming positive z-axis rotation means clockwise direction
    int gyro_z = Gyroscope_getAngle(); // Reuse the simplified angle as a proxy for direction
    if (gyro_z > 0)
    {
        return 1; // Clockwise
    }
    else if (gyro_z < 0)
    {
        return -1; // Counterclockwise
    }
    return 0; // No significant movement
}

// Example of how you might use these in
