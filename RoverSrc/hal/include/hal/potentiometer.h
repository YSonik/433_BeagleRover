#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_

void Potentiometer_init(void);

int Potentiometer_readValue(void);
double Potentiometer_readVoltage(void);
double Potentiometer_readPercentage(void);

void Potentiometer_cleanup(void);

#endif // _POTENTIOMETER_H_
