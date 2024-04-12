#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#include "utils/gpio_utils.h"
#include "utils/i2c_utils.h"
#include "hal/accelerometer.h"

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define ACCELEROMETER_ADDRESS 0x18

#define ACCELEROMETER_REG_WHO_AM_I 0x0F
#define ACCELEROMETER_REG_CTRL_REG1 0x20

#define ACCELEROMETER_X_AXIS_REG_LSB 0x28
#define ACCELEROMETER_X_AXIS_REG_MSB 0x29

#define ACCELEROMETER_Y_AXIS_REG_LSB 0x2A
#define ACCELEROMETER_Y_AXIS_REG_MSB 0x2B

#define ACCELEROMETER_Z_AXIS_REG_LSB 0x2C
#define ACCELEROMETER_Z_AXIS_REG_MSB 0x2D

#define ACCELEROMETER_CTRL_REG1_POWER_ON 0x27

#define SDA_PIN "P9_18"
#define SDA_GPIO_NUMBER "44"

#define SCL_PIN "P9_17"
#define SCL_GPIO_NUMBER "61"

#define DIGIT_COUNT 2

typedef struct Accelerometer_t
{
    char pins[DIGIT_COUNT][6];
    char gpio_numbers[DIGIT_COUNT][3];
} Accelerometer_t;

Accelerometer_t accelerometer = {
    {SDA_PIN, SCL_PIN},
    {SDA_GPIO_NUMBER, SCL_GPIO_NUMBER},
};

static bool is_initialized = false;
static int i2cFileDesc = 0;

static void Accelerometer_setPowerOn()
{
    writeI2cReg(i2cFileDesc, ACCELEROMETER_REG_CTRL_REG1, ACCELEROMETER_CTRL_REG1_POWER_ON);
}

void Accelerometer_init()
{
    assert(!is_initialized);

    for (int i = 0; i < DIGIT_COUNT; i++)
    {
        configurePinI2c(accelerometer.pins[i]);

        exportGpioPin(accelerometer.gpio_numbers[i]);
    }

    i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, ACCELEROMETER_ADDRESS);

    Accelerometer_setPowerOn();

    is_initialized = true;
}

void Accelerometer_cleanup()
{
    assert(is_initialized);

    for (int i = 0; i < DIGIT_COUNT; i++)
    {
        unexportGpioPin(accelerometer.gpio_numbers[i]);
    }

    is_initialized = false;
}

static int16_t Accelerometer_orderBytes(int lsb, int msb)
{
    int16_t x = (msb << 8) | lsb;

    return x >> 4; // 12-bit resolution, so we need to shift 4 bits to the right
}

static double Accelerometer_readValue(int reg, unsigned char lsb_address, unsigned char msb_address)
{
    char lsb = readI2cReg(reg, lsb_address);
    char msb = readI2cReg(reg, msb_address);

    int16_t x = Accelerometer_orderBytes(lsb, msb);

    return x / 1000.0; // Convert to g
}

double Accelerometer_getX()
{
    assert(is_initialized);

    return Accelerometer_readValue(i2cFileDesc, ACCELEROMETER_X_AXIS_REG_LSB, ACCELEROMETER_X_AXIS_REG_MSB);
}

double Accelerometer_getY()
{
    assert(is_initialized);

    return Accelerometer_readValue(i2cFileDesc, ACCELEROMETER_Y_AXIS_REG_LSB, ACCELEROMETER_Y_AXIS_REG_MSB);
}

double Accelerometer_getZ()
{
    assert(is_initialized);

    return Accelerometer_readValue(i2cFileDesc, ACCELEROMETER_Z_AXIS_REG_LSB, ACCELEROMETER_Z_AXIS_REG_MSB);
}

void Accelerometer_ReadAll(double *x, double *y, double *z)
{
    assert(is_initialized);

    unsigned char data[6];

    readI2cBlockData(i2cFileDesc, ACCELEROMETER_X_AXIS_REG_LSB + 0x80, data, 6 * sizeof(unsigned char));

    *x = Accelerometer_orderBytes(data[0], data[1]) / 1000.0;
    *y = Accelerometer_orderBytes(data[2], data[3]) / 1000.0;
    *z = Accelerometer_orderBytes(data[4], data[5]) / 1000.0;
}