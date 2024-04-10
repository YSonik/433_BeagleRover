#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "utils/gpio_utils.h"
#include "utils/i2c_utils.h"
#include "utils/file_utils.h"
#include "utils/time_utils.h"

#include "hal/segment_display.h"

#define DIGIT_COUNT 2
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x20

#define SEGMENT_ADDRESS_LOWER 0
#define SEGMENT_ADDRESS_UPPER 1

#define LEFT_DIGIT_PIN "61"
#define RIGHT_DIGIT_PIN "44"

#define LEFT_DIGIT 0
#define RIGHT_DIGIT 1

typedef struct SegmentDisplay_t
{
    char pins[DIGIT_COUNT][6];
    char gpio_numbers[DIGIT_COUNT][3];
    int output_registers[DIGIT_COUNT];
    int direction_registers[DIGIT_COUNT];
} SegmentDisplay;

static SegmentDisplay segmentDisplay = {
    {"P9_17", "P9_18"},
    {LEFT_DIGIT_PIN, RIGHT_DIGIT_PIN},
    {SEGMENT_ADDRESS_LOWER, SEGMENT_ADDRESS_UPPER},
    {0x02, 0x03},
};

static short SEGMENTS_DIGIT_MAP[10][2] = {
    {209, 229},
    {192, 4},
    {152, 131},
    {216, 3},
    {200, 34},
    {88, 35},
    {88, 163},
    {2, 5},
    {216, 227},
    {216, 99},
};

static bool isInitialized = false;

static pthread_t thread = 0;
static bool running = true;

static int i2cFileDesc = 0;
static int digits[DIGIT_COUNT] = {0};

static void turnOnDigit(int side)
{
    setGpioValue(segmentDisplay.gpio_numbers[side], "1");
}

static void turnOffDigit(int side)
{
    setGpioValue(segmentDisplay.gpio_numbers[side], "0");
}

static void setI2cDirection(int direction)
{
    writeI2cReg(i2cFileDesc, segmentDisplay.direction_registers[LEFT_DIGIT], direction);
    writeI2cReg(i2cFileDesc, segmentDisplay.direction_registers[RIGHT_DIGIT], direction);
}

static void writeDigit(int digit)
{
    writeI2cReg(i2cFileDesc, SEGMENT_ADDRESS_LOWER, SEGMENTS_DIGIT_MAP[digit][0]);
    writeI2cReg(i2cFileDesc, SEGMENT_ADDRESS_UPPER, SEGMENTS_DIGIT_MAP[digit][1]);
}

static void *SegmentDisplay_thread()
{
    while (running)
    {
        for (int i = 0; i < DIGIT_COUNT; i++)
        {
            turnOffDigit(LEFT_DIGIT);
            turnOffDigit(RIGHT_DIGIT);

            writeDigit(digits[i]);

            turnOnDigit(i);

            sleepForMs(2);
        }
    }

    return NULL;
}

void SegmentDisplay_init(void)
{
    assert(!isInitialized);

    for (int i = 0; i < DIGIT_COUNT; i++)
    {
        configurePinI2c(segmentDisplay.pins[i]);

        exportGpioPin(segmentDisplay.gpio_numbers[i]);
        setGpioDirection(segmentDisplay.gpio_numbers[i], "out");
    }
    i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

    setI2cDirection(0);

    pthread_create(&thread, NULL, SegmentDisplay_thread, NULL);

    isInitialized = true;
}

void SegmentDisplay_write(int number)
{
    assert(isInitialized);
    assert(number >= 0 && number <= 99);

    digits[0] = number / 10;
    digits[1] = number % 10;
}

void SegmentDisplay_stop(void)
{
    running = false;
}

void SegmentDisplay_cleanup(void)
{
    assert(isInitialized);

    pthread_join(thread, NULL);

    turnOffDigit(LEFT_DIGIT);
    turnOffDigit(RIGHT_DIGIT);

    close(i2cFileDesc);

    // for (int i = 0; i < DIGIT_COUNT; i++)
    // {
    //     unexportGpioPin(segmentDisplay.gpio_numbers[i]);
    // }

    isInitialized = false;
}
