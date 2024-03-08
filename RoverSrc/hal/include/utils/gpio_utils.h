#ifndef _GPIO_UTILS_H_
#define _GPIO_UTILS_H_

#include <stdbool.h>

bool isGpioPinExported(const char *gpio);

void exportGpioPin(const char *gpio);
void unexportGpioPin(const char *gpio);

void setGpioDirection(const char *gpio, const char *direction);
void setGpioEdge(const char *gpio, const char *edge);

void setGpioValue(const char *gpio, const char *value);
int getGpioValue(const char *gpio);

void enableGpioPin(const char *gpio);
void disableGpioPin(const char *gpio);

void setGpioActiveLow(const char *pin, int active_low);

void configurePin(const char *pin, const char *mode);
void configurePinGpio(const char *pin);

#endif