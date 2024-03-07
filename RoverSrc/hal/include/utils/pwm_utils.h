#ifndef __PWM_UTILS_H
#define __PWM_UTILS_H

void setPwmDutyCycle(char* path, int dutyCycle);
void setPwmPeriod(char* path, int period);

void enablePwm(char* path);
void disablePwm(char* path);

void configurePinPwm(const char *pin);

#endif