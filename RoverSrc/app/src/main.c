#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "utils/time_utils.h"
#include "hal/dfrobot_pirate.h"

int main()
{
    printf("Rover project compiles correctly\n");

    DFRobotPirate_init();

    DFRobotPirate_SetSpeed(50);
    DFRobotPirate_MoveForward();

    sleepForMs(2000);

    DFRobotPirate_MoveBackward();

    sleepForMs(2000);

    DFRobotPirate_TurnLeft();

    sleepForMs(2000);

    DFRobotPirate_TurnRight();

    sleepForMs(2000);

    DFRobotPirate_Stop();

    DFRobotPirate_cleanUp();

    return 0;
}