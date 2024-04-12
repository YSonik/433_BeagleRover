#include <stdio.h>

#include "utils/file_utils.h"
#include "utils/pwm_utils.h"
#include "utils/gpio_utils.h"

#define PERIOD_SUFFIX "/period"
#define DUTY_CYCLE_SUFFIX "/duty_cycle"
#define ENABLE_SUFFIX "/enable"

void setPwmDutyCycle(const char *path, int dutyCycle)
{
    char duty_cycle_path[30];
    snprintf(duty_cycle_path, 30, "%s%s", path, DUTY_CYCLE_SUFFIX);

    char valueStr[11];
    snprintf(valueStr, 11, "%d", dutyCycle);

    writeToFile(duty_cycle_path, valueStr);
}

void setPwmPeriod(const char *path, int period)
{
    char period_path[25];
    snprintf(period_path, 25, "%s%s", path, PERIOD_SUFFIX);

    char valueStr[11];
    snprintf(valueStr, 11, "%d", period);

    writeToFile(period_path, valueStr);
}

void setPwmEnable(const char *path, int enable)
{
    char enable_path[26];
    snprintf(enable_path, 26, "%s%s", path, ENABLE_SUFFIX);

    char valueStr[2];
    snprintf(valueStr, 2, "%d", enable);

    writeToFile(enable_path, valueStr);
}

void enablePwm(const char *path)
{
    setPwmEnable(path, 1);
}

void disablePwm(const char *path)
{
    setPwmEnable(path, 0);
}

void configurePinPwm(const char *pin)
{
    configurePin(pin, GPIO_PWM_MODE);
}
