#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <assert.h>

#include "utils/gpio_utils.h"
#include "utils/time_utils.h"
#include "utils/pwm_utils.h"

#include "hal/dfrobot_pirate.h"

#define MOTOR_GROUP_COUNT 2

#define LEFT_MOTOR_FORWARD_PIN "p9.15"
#define LEFT_MOTOR_BACKWARD_GPIO_NUMBER "48"

#define LEFT_MOTOR_BACKWARD_PIN "p9.17"
#define LEFT_MOTOR_FORWARD_GPIO_NUMBER "5"

#define RIGHT_MOTOR_FORWARD_PIN "p9.16"
#define RIGHT_MOTOR_BACKWARD_GPIO_NUMBER "51"

#define RIGHT_MOTOR_BACKWARD_PIN "p9.18"
#define RIGHT_MOTOR_FORWARD_GPIO_NUMBER "4"

#define LEFT_MOTOR_PWM_PIN "p9.21"
#define LEFT_MOTOR_PWM_GPIO_NUMBER "3"

#define RIGHT_MOTOR_PWM_PIN "p9.14"
#define RIGHT_MOTOR_PWM_GPIO_NUMBER "50"

#define LEFT_PWM_PATH "/dev/bone/pwm/0/b"
#define RIGHT_PWM_PATH "/dev/bone/pwm/1/a"

#define PWM_PERIOD 100000000

#define MOTOR_DIRECTION_FORWARD 0
#define MOTOR_DIRECTION_BACKWARD 1
#define MOTOR_DIRECTION_STOP 2

#define GPIO_HIGH "high"
#define GPIO_LOW "low"

#define LEFT 0
#define RIGHT 1

static struct MotorPins
{
    const char *forward_pin;
    const char *forward_gpio_number;
    const char *backward_pin;
    const char *backward_gpio_number;

    const char *pwm_pin;
    const char *pwm_gpio_number;
    const char *pwm_path;
} motor_pins[] = {
    {
        LEFT_MOTOR_FORWARD_PIN,
        LEFT_MOTOR_FORWARD_GPIO_NUMBER,
        LEFT_MOTOR_BACKWARD_PIN,
        LEFT_MOTOR_BACKWARD_GPIO_NUMBER,

        LEFT_MOTOR_PWM_PIN,
        LEFT_MOTOR_PWM_GPIO_NUMBER,
        LEFT_PWM_PATH,
    },
    {
        RIGHT_MOTOR_FORWARD_PIN,
        RIGHT_MOTOR_FORWARD_GPIO_NUMBER,
        RIGHT_MOTOR_BACKWARD_PIN,
        RIGHT_MOTOR_BACKWARD_GPIO_NUMBER,

        RIGHT_MOTOR_PWM_PIN,
        RIGHT_MOTOR_PWM_GPIO_NUMBER,
        RIGHT_PWM_PATH,
    },
};

static bool is_initialized = false;

static int current_speed = 0;

static void setMotorDirection(int motor, int direction)
{

    if (motor < 0 || motor >= MOTOR_GROUP_COUNT)
    {
        return;
    }

    if (direction == MOTOR_DIRECTION_FORWARD)
    {
        setGpioDirection(motor_pins[motor].forward_gpio_number, GPIO_HIGH);
        setGpioDirection(motor_pins[motor].backward_gpio_number, GPIO_LOW);
    }
    else if (direction == MOTOR_DIRECTION_BACKWARD)
    {
        setGpioDirection(motor_pins[motor].forward_gpio_number, GPIO_LOW);
        setGpioDirection(motor_pins[motor].backward_gpio_number, GPIO_HIGH);
    }
    else
    {
        setGpioDirection(motor_pins[motor].forward_gpio_number, GPIO_LOW);
        setGpioDirection(motor_pins[motor].backward_gpio_number, GPIO_LOW);
    }
}

static void setDirection(int left, int right)
{
    setMotorDirection(LEFT, left);
    setMotorDirection(RIGHT, right);}

void DFRobotPirate_init()
{
    assert(!is_initialized);

    for (int i = 0; i < MOTOR_GROUP_COUNT; i++)
    {
        exportGpioPin(motor_pins[i].forward_gpio_number);
        exportGpioPin(motor_pins[i].backward_gpio_number);
        exportGpioPin(motor_pins[i].pwm_gpio_number);

        configurePinGpio(motor_pins[i].forward_pin);
        configurePinGpio(motor_pins[i].backward_pin);
        configurePinPwm(motor_pins[i].pwm_pin);

        setGpioDirection(motor_pins[i].forward_gpio_number, GPIO_LOW);
        setGpioDirection(motor_pins[i].backward_gpio_number, GPIO_LOW);

        setGpioActiveLow(motor_pins[i].forward_gpio_number, 0);
        setGpioActiveLow(motor_pins[i].backward_gpio_number, 0);
        setGpioActiveLow(motor_pins[i].pwm_gpio_number, 0);

        setPwmDutyCycle(motor_pins[i].pwm_path, 0);
        setPwmPeriod(motor_pins[i].pwm_path, PWM_PERIOD);

        enablePwm(motor_pins[i].pwm_path);
    }

    is_initialized = true;
}

void DFRobotPirate_cleanup()
{
    assert(is_initialized);

    // for (int i = 0; i < MOTOR_GROUP_COUNT; i++)
    // {
    //     unexportGpioPin(motor_pins[i].forward_gpio_number);
    //     unexportGpioPin(motor_pins[i].backward_gpio_number);
    //     unexportGpioPin(motor_pins[i].pwm_gpio_number);
    // }

    is_initialized = false;
}

void DFRobotPirate_turnLeft()
{
    assert(is_initialized);

    setDirection(MOTOR_DIRECTION_FORWARD, MOTOR_DIRECTION_BACKWARD);
}

void DFRobotPirate_turnRight()
{
    assert(is_initialized);

    setDirection(MOTOR_DIRECTION_BACKWARD, MOTOR_DIRECTION_FORWARD);
}

void DFRobotPirate_moveForward()
{
    assert(is_initialized);

    setDirection(MOTOR_DIRECTION_FORWARD, MOTOR_DIRECTION_FORWARD);
}

void DFRobotPirate_moveBackward()
{
    assert(is_initialized);

    setDirection(MOTOR_DIRECTION_BACKWARD, MOTOR_DIRECTION_BACKWARD);
}

int DFRobotPirate_getSpeed()
{
    assert(is_initialized);

    return current_speed;
}

void DFRobotPirate_setSpeed(int speed)
{
    assert(is_initialized);
    assert(speed >= 0 && speed <= 100);

    if (speed == current_speed)
    {
        return;
    }

    setPwmDutyCycle(motor_pins[0].pwm_path, (PWM_PERIOD * speed) / 100);
    setPwmDutyCycle(motor_pins[1].pwm_path, (PWM_PERIOD * speed) / 100);

    current_speed = speed;
}

void DFRobotPirate_stop()
{
    assert(is_initialized);

    setDirection(MOTOR_DIRECTION_STOP, MOTOR_DIRECTION_STOP);
}