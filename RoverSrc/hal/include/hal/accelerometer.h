#ifndef ACCEL_H
#define ACCEL_H

void Accel_init(void);
void Accel_cleanup(void);
// void getAccelerometerValues();
void getAccelerometerValues(int16_t *xAccl, int16_t *yAccl);

#endif