#ifndef __ACCELEROMETER_H
#define __ACCELEROMETER_H

void Accelerometer_init(void);
void Accelerometer_cleanUp(void);

int Accelerometer_getX(void);
int Accelerometer_getY(void);
int Accelerometer_getZ(void);

#endif // __ACCELEROMETER_H