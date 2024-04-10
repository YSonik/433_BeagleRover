#ifndef __ACCELEROMETER_H
#define __ACCELEROMETER_H

void Accelerometer_init(void);
void Accelerometer_cleanup(void);

double Accelerometer_getX(void);
double Accelerometer_getY(void);
double Accelerometer_getZ(void);

void Accelerometer_ReadAll(double *x, double *y, double *z);

#endif // __ACCELEROMETER_H