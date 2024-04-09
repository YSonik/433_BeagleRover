#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../hal/include/hal/accelerometer.h"
#include "../../hal/include/hal/gyroscope.h"
#include <time.h>


#include "utils/time_utils.h"
#include "hal/dfrobot_pirate.h"

int main()
{
    printf("Rover project compiles correctly\n");
    Gyroscope_init();

    int16_t xAcclValue;

    while (1)
    {
        Gyroscope_getAngle(&xAcclValue);
        float xAcclValue_float = xAcclValue / 1000.0f;
        printf("xGyroValue: %.3f\n", xAcclValue_float);
    }
    

    // Accel_init();

    // int16_t xAcclValue, yAcclValue;

    // getAccelerometerValues(&xAcclValue, &yAcclValue);

    // while (1)
    // {
    //     getAccelerometerValues(&xAcclValue, &yAcclValue);
    //     float xAcclValue_float = xAcclValue / 1000.0f;
    // float yAcclValue_float = yAcclValue / 1000.0f;

    // // Print accelerometer values
    // printf("xAcclValue: %.3f\n", xAcclValue_float);
    // printf("yAcclValue: %.3f\n", yAcclValue_float);
    // }

    return 0;
}