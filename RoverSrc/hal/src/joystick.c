#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#include "utils/file_utils.h"
#include "utils/time_utils.h"
#include "utils/gpio_utils.h"

#include "hal/joystick.h"

#define GPIO_PATH "/sys/class/gpio/gpio"
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"

#define DIRECTION_SUFFIX "/direction"
#define EDGE_SUFFIX "/edge"
#define VALUE_SUFFIX "/value"

#define IO_DIRECTION "in"

char *edge_directions[] = {
    "rising",
    "falling",
    "both",
};

#define GPIO_CHILD_PATH_FORMAT "%s%s%s"

#define JOYSTICK_UP_GPIO_NUMBER "26"
#define JOYSTICK_UP_PIN "p8.14"

#define JOYSTICK_DOWN_GPIO_NUMBER "46"
#define JOYSTICK_DOWN_PIN "p8.15"

#define JOYSTICK_LEFT_GPIO_NUMBER "65"
#define JOYSTICK_LEFT_PIN "p8.16"

#define JOYSTICK_RIGHT_GPIO_NUMBER "47"
#define JOYSTICK_RIGHT_PIN "p8.17"
static struct JoystickPinDetails
{
    const char *gpio_number;
    const char *pin;
    const char *name;
    const direction direction;
    int fd;
} joystick_pins[] = {
    {JOYSTICK_UP_GPIO_NUMBER, JOYSTICK_UP_PIN, "Up", JOYSTICK_UP, 0},
    {JOYSTICK_DOWN_GPIO_NUMBER, JOYSTICK_DOWN_PIN, "Down", JOYSTICK_DOWN, 0},
    {JOYSTICK_LEFT_GPIO_NUMBER, JOYSTICK_LEFT_PIN, "Left", JOYSTICK_LEFT, 0},
    {JOYSTICK_RIGHT_GPIO_NUMBER, JOYSTICK_RIGHT_PIN, "Right", JOYSTICK_RIGHT, 0},
};

static bool is_initialized = false;

static int epoll_fd;

#define MAX_EVENTS 4
static struct epoll_event events[MAX_EVENTS];

static int create_epoll_fd(const char *pin)
{
    int gpio_fd = open(pin, O_RDONLY | O_NONBLOCK);

    if (gpio_fd == -1)
    {
        printf("ERROR: Unable to open gpio fd\n");
        exit(-1);
    }

    return gpio_fd;
}

static void register_epoll_fd(int fd, int id)
{
    events[id].events = EPOLLIN | EPOLLET | EPOLLPRI;
    events[id].data.fd = fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &events[id]) == -1)
    {
        printf("ERROR: Unable to register epoll fd\n");
        exit(-1);
    }
}

static void close_fd(int fd)
{
    if (close(fd) == -1)
    {
        printf("ERROR: Unable to close epoll fd\n");
        exit(-1);
    }
}

static void setupGpio(struct JoystickPinDetails *joystick_pin, int index)
{
    configurePinGpio(joystick_pin->pin);

    if (!isExported(joystick_pin->gpio_number))
    {
        exportGpio(joystick_pin->gpio_number);
    }

    setGpioDirection(joystick_pin->gpio_number, IO_DIRECTION);

    char value[29];
    snprintf(value, 29, GPIO_CHILD_PATH_FORMAT, GPIO_PATH, joystick_pin->gpio_number, VALUE_SUFFIX);

    int fd = create_epoll_fd(value);

    register_epoll_fd(fd, index);

    joystick_pin->fd = fd;
}

void Joystick_init()
{
    assert(!is_initialized);

    epoll_fd = epoll_create(1);
    if (epoll_fd == -1)
    {
        printf("ERROR: Unable to create epoll fd\n");
        exit(-1);
    }

    for (int i = 0; i < 4; i++)
    {
        struct JoystickPinDetails *joystick_pin = &joystick_pins[i];
        setupGpio(joystick_pin, i);
    }

    is_initialized = true;

    // ignoring first input
    Joystick_waitForInput(0, FALLING);
}

const char *Joystick_getDirectionString(direction dir)
{
    return (char *)joystick_pins[dir].name;
}

direction Joystick_waitForInput(int timeout_ms, edge e)
{
    if (!is_initialized)
    {
        printf("ERROR: Joystick not initialized\n");
        exit(-1);
    }

    for (int i = 0; i < 4; i++)
    {
        struct JoystickPinDetails *joystick_pin = &joystick_pins[i];

        setGpioEdge(joystick_pin->gpio_number, edge_directions[e]);
    }

    int epoll_wait_result = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout_ms);
    if (epoll_wait_result == -1)
    {
        printf("ERROR: Unable to wait for epoll fd\n");
        exit(-1);
    }

    direction joystick_direction = JOYSTICK_NONE;

    for (int i = 0; i < epoll_wait_result; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            const struct JoystickPinDetails *joystick_pin = &joystick_pins[j];
            if (events[i].data.fd == joystick_pin->fd)
            {
                joystick_direction = joystick_pin->direction;
                break;
            }
        }
    }

    return joystick_direction;
}

direction Joystick_read()
{
    if (!is_initialized)
    {
        printf("ERROR: Joystick not initialized\n");
        exit(-1);
    }

    direction joystick_direction = JOYSTICK_NONE;

    for (int j = 0; j < 4; j++)
    {
        const struct JoystickPinDetails *joystick_pin = &joystick_pins[j];

        int value = getGpioValue(joystick_pin->pin);

        if (value == 0)
        {
            joystick_direction = joystick_pin->direction;
        }
    }

    return joystick_direction;
}

void Joystick_cleanUp()
{
    if (!is_initialized)
    {
        printf("ERROR: Joystick not initialized\n");
        exit(-1);
    }

    for (int i = 0; i < 4; i++)
    {
        struct JoystickPinDetails *joystick_pin = &joystick_pins[i];
        close_fd(joystick_pin->fd);
    }

    close_fd(epoll_fd);

    is_initialized = false;
}