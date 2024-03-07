#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <assert.h>

#include "utils/gpio_utils.h"
#include "utils/time_utils.h"
#include "utils/pwm_utils.h"

#include "hal/dfrobot_pirate.h"

#define MOTOR_COUNT 4

#define LEFT_MOTOR_FORWARD_PIN "p9.15"
#define LEFT_MOTOR_BACKWARD_PIN "p9.17"

#define RIGHT_MOTOR_FORWARD_PIN "p9.18"
#define RIGHT_MOTOR_BACKWARD_PIN "p9.21"

#define LEFT_MOTOR_PWM_PIN "p9.14"
#define RIGHT_MOTOR_PWM_PIN "p9.16"

#define LEFT_PWM_PATH "/dev/bone/pwm/1/a"
#define RIGHT_PWM_PATH "/dev/bone/pwm/1/b/"

#define PWM_PERIOD 500000
#define PWM_DUTY_CYCLE 250000

#define MOTOR_DIRECTION_FORWARD 0
#define MOTOR_DIRECTION_BACKWARD 1

#define GPIO_HIGH "HIGH"
#define GPIO_LOW "LOW"

static struct MotorPins
{
    const char *forward_pin;
    const char *backward_pin;
    const char *pwm_pin;
    const char *pwm_path;
} motor_pins[] = {
    {
        LEFT_MOTOR_FORWARD_PIN,
        LEFT_MOTOR_BACKWARD_PIN,
        LEFT_MOTOR_PWM_PIN,
        LEFT_PWM_PATH,
    },
    {
        RIGHT_MOTOR_FORWARD_PIN,
        RIGHT_MOTOR_BACKWARD_PIN,
        RIGHT_MOTOR_PWM_PIN,
        RIGHT_PWM_PATH,
    },
};

static bool is_initialized = false;

static int current_speed = 0;

static void setMotorDirection(int motor, int direction)
{

    if (motor < 0 || motor >= MOTOR_COUNT)
    {
        return;
    }

    if (direction == MOTOR_DIRECTION_FORWARD)
    {
        setGpioDirection(motor_pins[motor].forward_pin, GPIO_HIGH);
        setGpioDirection(motor_pins[motor].backward_pin, GPIO_LOW);
    }
    else if (direction == MOTOR_DIRECTION_BACKWARD)
    {
        setGpioDirection(motor_pins[motor].forward_pin, GPIO_LOW);
        setGpioDirection(motor_pins[motor].backward_pin, GPIO_HIGH);
    }
    else
    {
        setGpioDirection(motor_pins[motor].forward_pin, GPIO_LOW);
        setGpioDirection(motor_pins[motor].backward_pin, GPIO_LOW);
    }
}

void DFRobotPirate_init()
{
    assert(!is_initialized);

    for (int i = 0; i < MOTOR_COUNT; i++)
    {
        exportGpioPin(motor_pins[i].forward_pin);
        exportGpioPin(motor_pins[i].backward_pin);
        exportGpioPin(motor_pins[i].pwm_pin);

        configurePinGpio(motor_pins[i].forward_pin);
        configurePinGpio(motor_pins[i].backward_pin);
        configurePinPwm(motor_pins[i].pwm_pin);

        setGpioDirection(motor_pins[i].forward_pin, GPIO_LOW);
        setGpioDirection(motor_pins[i].backward_pin, GPIO_LOW);
    }

    is_initialized = true;
}

void DFRobotPirate_cleanUp()
{
    assert(is_initialized);

    for (int i = 0; i < MOTOR_COUNT; i++)
    {
        unexportGpioPin(motor_pins[i].forward_pin);
        unexportGpioPin(motor_pins[i].backward_pin);
        unexportGpioPin(motor_pins[i].pwm_pin);
    }

    is_initialized = false;
}

void DFRobotPirate_TurnLeft()
{
    assert(is_initialized);

    setMotorDirection(0, MOTOR_DIRECTION_BACKWARD);
    setMotorDirection(1, MOTOR_DIRECTION_FORWARD);
}

void DFRobotPirate_TurnRight()
{
    assert(is_initialized);

    setMotorDirection(0, MOTOR_DIRECTION_FORWARD);
    setMotorDirection(1, MOTOR_DIRECTION_BACKWARD);
}

void DFRobotPirate_MoveForward()
{
    assert(is_initialized);

    setMotorDirection(0, MOTOR_DIRECTION_FORWARD);
    setMotorDirection(1, MOTOR_DIRECTION_FORWARD);
}

void DFRobotPirate_MoveBackward()
{
    assert(is_initialized);

    setMotorDirection(0, MOTOR_DIRECTION_BACKWARD);
    setMotorDirection(1, MOTOR_DIRECTION_BACKWARD);
}

void DFRobotPirate_SetSpeed(int speed)
{
    assert(is_initialized);
    assert(speed >= 0 && speed <= 100);

    if (speed == current_speed)
    {
        return;
    }

    setPwmPeriod(motor_pins[0].pwm_path, PWM_PERIOD);
    setPwmPeriod(motor_pins[1].pwm_path, PWM_PERIOD);

    setPwmDutyCycle(motor_pins[0].pwm_path, PWM_DUTY_CYCLE * speed / 100);
    setPwmDutyCycle(motor_pins[1].pwm_path, PWM_DUTY_CYCLE * speed / 100);
}

void DFRobotPirate_Stop()
{
    assert(is_initialized);

    for (int i = 0; i < MOTOR_COUNT; i++)
    {
        setGpioDirection(motor_pins[i].forward_pin, GPIO_LOW);
        setGpioDirection(motor_pins[i].backward_pin, GPIO_LOW);
    }
}