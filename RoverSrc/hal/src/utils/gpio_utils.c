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
#define ACTIVE_LOW_SUFFIX "/active_low"

#define ENABLE_SUFFIX "/enable"

#define GPIO_CHILD_PATH_FORMAT "%s%s%s"

bool isGpioPinExported(const char *pin)
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

void exportGpioPin(const char *pin)
{
    if (isGpioPinExported(pin))
    {
        return;
    }

    writeToFile(GPIO_EXPORT_PATH, pin);

    sleepForMs(300);
}

void unexportGpioPin(const char *pin)
{
    if (!isGpioPinExported(pin))
    {
        return;
    }

    writeToFile(GPIO_UNEXPORT_PATH, pin);
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

int getGpioValue(const char *pin)
{
    char value_path[35];
    snprintf(value_path, 35, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, pin, VALUE_SUFFIX);

    char buff[10];
    readLineFromFile(value_path, buff, 10);

    return atoi(buff);
}

void enableGpioPin(const char *pin)
{
    char enable_path[35];
    snprintf(enable_path, 35, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, pin, ENABLE_SUFFIX);

    writeToFile(enable_path, "1");
}

void disableGpioPin(const char *pin)
{
    char enable_path[35];
    snprintf(enable_path, 35, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, pin, ENABLE_SUFFIX);

    writeToFile(enable_path, "0");
}

void setGpioActiveLow(const char *pin, int active_low)
{
    char active_low_path[35];
    snprintf(active_low_path, 35, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, pin, ACTIVE_LOW_SUFFIX);

    char active_low_str[2];
    snprintf(active_low_str, 2, "%d", active_low);

    writeToFile(active_low_path, active_low_str);
}

void configurePin(const char *pin, const char *mode)
{
    char config_command[30];
    snprintf(config_command, 30, "config-pin %s %s", pin, mode);

    runCommand(config_command);
}

void configurePinGpio(const char *pin)
{
    configurePin(pin, GPIO_GPIO_MODE);
}
