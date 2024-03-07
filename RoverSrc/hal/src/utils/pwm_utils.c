#include "utils/file_utils.h"
#include "utils/pwm_utils.h"
#include "utils/gpio_utils.h"

#define PERIOD_SUFFIX "/period"
#define DUTY_CYCLE_SUFFIX "/duty_cycle"
#define ENABLE_SUFFIX "/enable"

void setPwmDutyCycle(const char path, int duty_cycle)
{
    char duty_cycle_path[256];
    sprintf(duty_cycle_path, "%s%s", path, DUTY_CYCLE_SUFFIX);

    writeToFile(duty_cycle_path, duty_cycle);
}

void setPwmPeriod(const char path, int period)
{
    char period_path[256];
    sprintf(period_path, "%s%s", path, PERIOD_SUFFIX);

    writeToFile(period_path, period);
}

void setPwmEnable(const char path, int enable)
{
    char enable_path[256];
    sprintf(enable_path, "%s%s", path, ENABLE_SUFFIX);

    writeToFile(enable_path, enable);
}

void configurePinPwm(const char *pin)
{
    configurePin(pin, "pwm");
}
