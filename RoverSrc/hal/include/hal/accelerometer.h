// #ifndef __ACCELEROMETER_H
// #define __ACCELEROMETER_H

// void Accelerometer_init(void);
// void Accelerometer_cleanUp(void);

// int Accelerometer_getX(void);
// int Accelerometer_getY(void);
// int Accelerometer_getZ(void);

// #endif // __ACCELEROMETER_H
#ifndef ACCEL_H
#define ACCEL_H

void Accel_init(void);
void Accel_cleanup(void);
// void getAccelerometerValues();
void getAccelerometerValues(int16_t *xAccl, int16_t *yAccl);

#endif