#define _POSIX_C_SOURCE 200809L
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h> // Errors
#include <string.h>

#include <sys/epoll.h> // for epoll()
#include <fcntl.h>     // for open()
#include <unistd.h>    // for close()

#include "utils/file_utils.h"
#include "utils/time_utils.h"
#include "utils/gpio_utils.h"

#define GPIO_PATH "/sys/class/gpio/gpio"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_UNEXPORT_PATH "/sys/class/gpio/unexport"

#define DIRECTION_SUFFIX "/direction"
#define EDGE_SUFFIX "/edge"
#define VALUE_SUFFIX "/value"
#define ENABLE_SUFFIX "/enable"

#define GPIO_CHILD_PATH_FORMAT "%s%s%s"

bool isExported(const char *pin)
{
    char gpio_path[33];
    snprintf(gpio_path, 33, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, pin, DIRECTION_SUFFIX);

    int fd = open(gpio_path, O_RDONLY);
    if (fd == -1)
    {
        return false;
    }

    if (close(fd) == -1)
    {
        printf("ERROR: Unable to close gpio export\n");
        exit(-1);
    }

    return true;
}

void exportGpio(const char *pin)
{
    if (isExported(pin))
    {
        return;
    }

    writeToFile(GPIO_EXPORT_PATH, pin);

    sleepForMs(300);
}

void setGpioDirection(const char *pin, const char *direction)
{
    char direction_path[33];
    snprintf(direction_path, 33, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, pin, DIRECTION_SUFFIX);

    writeToFile(direction_path, direction);
}

void setGpioEdge(const char *pin, const char *edge)
{
    char edge_path[28];
    snprintf(edge_path, 28, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, pin, EDGE_SUFFIX);

    writeToFile(edge_path, edge);
}

void setGpioValue(const char *pin, const char *value)
{
    char value_path[35];
    snprintf(value_path, 35, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, pin, VALUE_SUFFIX);

    writeToFile(value_path, value);
}

void enableGpio(const char *pin)
{
    char enable_path[35];
    snprintf(enable_path, 35, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, pin, ENABLE_SUFFIX);

    writeToFile(enable_path, "1");
}

void disableGpio(const char *pin)
{
    char enable_path[35];
    snprintf(enable_path, 35, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, pin, ENABLE_SUFFIX);

    writeToFile(enable_path, "0");
}

int getGpioValue(const char *pin)
{
    char value_path[35];
    snprintf(value_path, 35, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, pin, VALUE_SUFFIX);

    char buff[10];
    readLineFromFile(value_path, buff, 10);

    return atoi(buff);
}

void unexportGpio(const char *pin)
{
    if (!isExported(pin))
    {
        return;
    }

    writeToFile(GPIO_UNEXPORT_PATH, pin);
}

void configurePin(const char *pin, const char *mode)
{
    char config_command[30];
    snprintf(config_command, 30, "config-pin %s %s", pin, mode);

    runCommand(config_command);
}

void configurePinGpio(const char *pin)
{
    configurePin(pin, "gpio");
}

void configurePinPwm(const char *pin)
{
    configurePin(pin, "pwm");
}

void configurePinI2c(const char *pin)
{
    configurePin(pin, "i2c");
}