#ifndef __PWM_UTILS_H
#define __PWM_UTILS_H

void setPwmDutyCycle(const char *path, int dutyCycle);
void setPwmPeriod(const char *path, int period);
void setPwmEnable(const char *path, int enable);

void enablePwm(const char *path);
void disablePwm(const char *path);

void configurePinPwm(const char *pin);

#endif