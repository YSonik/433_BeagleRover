#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "hal/potentiometer.h"
#include "utils/file_utils.h"

#define DEVICE_PATH "/sys/bus/iio/devices/iio:device0"
#define POTENTIOMETER_PIN "P9_39"
#define POTENTIOMETER_NUMBER "0"
#define POTENTIOMETER_MAX 4095
#define POTENTIOMETER_REF_VOLTAGE 1.8

static bool isInitialized = false;

void Potentiometer_init(void)
{
    assert(!isInitialized);

    isInitialized = true;
}

int Potentiometer_readValue(void)
{
    assert(isInitialized);

    char buffer[10];

    readLineFromFile(DEVICE_PATH "/in_voltage" POTENTIOMETER_NUMBER "_raw", buffer, sizeof(buffer));

    int value = atoi(buffer);
    return value;
}

double Potentiometer_readVoltage(void)
{
    assert(isInitialized);

    int value = Potentiometer_readValue();
    double voltage = (value / (double)POTENTIOMETER_MAX) * POTENTIOMETER_REF_VOLTAGE;
    return voltage;
}

double Potentiometer_readPercentage(void)
{
    assert(isInitialized);

    int value = Potentiometer_readValue();
    double percentage = (value / (double)POTENTIOMETER_MAX) * 100;

    return percentage;
}

void Potentiometer_cleanup(void)
{
    assert(isInitialized);
    isInitialized = false;
}