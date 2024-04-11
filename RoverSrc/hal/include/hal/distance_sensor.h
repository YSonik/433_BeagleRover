#ifndef __DISTANCE_SENSOR_H
#define __DISTANCE_SENSOR_H

void DistanceSensor_init(void);
void DistanceSensor_cleanup(void);
float getFilteredDistance(void);

#endif // __DISTANCE_SENSOR_H
