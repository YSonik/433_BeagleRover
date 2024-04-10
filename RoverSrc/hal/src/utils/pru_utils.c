#include "utils/gpio_utils.h"
#include "utils/pru_utils.h"

void configurePinPruOut(const char *pin)
{
    configurePin(pin, GPIO_PRU_OUT_MODE);
}

void configurePinPruIn(const char *pin)
{
    configurePin(pin, GPIO_PRU_IN_MODE);
}