#ifndef _GPIO_UTILS_H_
#define _GPIO_UTILS_H_

bool isExported(const char *gpio);

void exportGpio(const char *gpio);
void unexportGpio(const char *gpio);

void setGpioDirection(const char *gpio, const char *direction);
void setGpioEdge(const char *gpio, const char *edge);
void setGpioValue(const char *gpio, const char *value);
int getGpioValue(const char *gpio);
void enableGpio(const char *gpio);
void disableGpio(const char *gpio);

void configurePin(const char *pin, const char *mode);
void configurePinGpio(const char *pin);
void configurePinPwm(const char *pin);
void configurePinI2c(const char *pin);

#endif