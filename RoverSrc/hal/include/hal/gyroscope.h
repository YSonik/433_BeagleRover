#ifndef __GYROSCOPE_H
#define __GYROSCOPE_H

void Gyroscope_init(void);
void Gyroscope_cleanup(void);

int Gyroscope_getAngle(void);
int Gyroscope_getDirection(void);

#endif // __GYROSCOPE_H