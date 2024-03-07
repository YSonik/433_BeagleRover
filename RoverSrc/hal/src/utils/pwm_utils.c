#include <stdio.h>

#include "utils/file_utils.h"
#include "utils/pwm_utils.h"
#include "utils/gpio_utils.h"

#define PERIOD_SUFFIX "/period"
#define DUTY_CYCLE_SUFFIX "/duty_cycle"
#define ENABLE_SUFFIX "/enable"

void setPwmDutyCycle(const char *path, int dutyCycle)
{
    char duty_cycle_path[256];
    sprintf(duty_cycle_path, "%s%s", path, DUTY_CYCLE_SUFFIX);

    char valueStr[11];
    sprintf(valueStr, "%d", dutyCycle);

    writeToFile(duty_cycle_path, valueStr);
}

void setPwmPeriod(const char *path, int period)
{
    char period_path[256];
    sprintf(period_path, "%s%s", path, PERIOD_SUFFIX);

    char valueStr[11];
    sprintf(valueStr, "%d", period);

    writeToFile(period_path, valueStr);
}

void setPwmEnable(const char *path, int enable)
{
    char enable_path[256];
    sprintf(enable_path, "%s%s", path, ENABLE_SUFFIX);

    char valueStr[2];
    sprintf(valueStr, "%d", enable);

    writeToFile(enable_path, valueStr);
}

void configurePinPwm(const char *pin)
{
    configurePin(pin, "pwm");
}
