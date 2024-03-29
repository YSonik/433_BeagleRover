#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <assert.h>

#include "utils/gpio_utils.h"
#include "utils/time_utils.h"

#include "hal/distance_sensor.h"

#define ECHO_GPIO_PATH "/sys/class/gpio/gpio48"
#define TRIGGER_GPIO_PATH "/sys/class/gpio/gpio49"

#define ECHO_PIN "p9.15"
#define ECHO_GPIO "48"

#define TRIGGER_PIN "p9.23"
#define TRIGGER_GPIO "49"

#define TRIGGER_HIGH "1"
#define TRIGGER_LOW "0"

static bool is_initialized = false;

void DistanceSensor_init()
{
    printf("initializing distance sensor\n");
    assert(!is_initialized);

    exportGpioPin(ECHO_GPIO);
    exportGpioPin(TRIGGER_GPIO);

    configurePinGpio(ECHO_PIN);
    configurePinGpio(TRIGGER_PIN);

    setGpioDirection(ECHO_GPIO, "in");
    setGpioDirection(TRIGGER_GPIO, "out");

    is_initialized = true;

}

void DistanceSensor_cleanup()
{
    assert(is_initialized);

    is_initialized = false;
}

float DistanceSensor_getDistance()
{
    assert(is_initialized);
    //printf("setting trigger to high\n");
    setGpioValue(TRIGGER_GPIO, TRIGGER_HIGH);

    sleepForNs(10000);
    //printf("setting trigger to low\n");
    setGpioValue(TRIGGER_GPIO, TRIGGER_LOW);

    long long startTime = getTimeInMs();
    long long endTime = getTimeInMs();

    while (getGpioValue(ECHO_GPIO) == 0) {
        startTime = getTimeInMs();
        //printf("startTime %lld\n", startTime);
    }

    while(getGpioValue(ECHO_GPIO) == 1) {
        endTime = getTimeInMs();
        //printf("endTime %lld\n", endTime);
    }

    float pulseDuration = (endTime - startTime) * 0.001;
    printf("pusle duration %f\n", pulseDuration);
    float distance = (pulseDuration * 34300) / 2.0;

    return distance;


}