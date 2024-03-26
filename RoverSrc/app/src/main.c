#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "utils/time_utils.h"
#include "hal/dfrobot_pirate.h"
#include "hal/car_server.h"
#include "hal/car_client.h"
#include "hal/joystick.h"

static bool isServer = true;


int main(int argc, char* argv[])
{
    printf("Rover project compiles correctly\n");

    if(strstr(argv[1], "false") != NULL)
    {
        isServer = false;
    }

    //Start all modules
    DFRobotPirate_init();
    Joystick_init();

    if(isServer)
    {
        UDP_server_start();
    }
    else
    {
        UDP_client_start();
    }



    sleepForMs(5000);





    if(isServer)
    {
        UDP_server_stop();
    }
    else
    {
        UDP_client_stop();
    }


    Joystick_cleanUp();
    DFRobotPirate_cleanUp();

    // DFRobotPirate_init();

    // DFRobotPirate_SetSpeed(20);
    // DFRobotPirate_MoveForward();

    // sleepForMs(5000);

    // DFRobotPirate_MoveBackward();

    // sleepForMs(5000);

    // DFRobotPirate_TurnLeft();

    // sleepForMs(5000);

    // DFRobotPirate_TurnRight();

    // sleepForMs(5000);

    // DFRobotPirate_Stop();

    // DFRobotPirate_cleanUp();

    return 0;
}