#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#include "utils/time_utils.h"
#include "hal/dfrobot_pirate.h"
#include "hal/joystick.h"

#include "socket.h"
#include "shutdown.h"
#include "server.h"
#include "client.h"

static bool killRequested = false;

static void signalHandler(int signal)
{
    printf("Received signal %d\n", signal);

    if (killRequested)
    {
        printf("Force killing\n");
        exit(1);
    }

    killRequested = true;
    Shutdown_request();
}

int main(int argc, char *argv[])
{
    printf("Rover project compiles correctly\n");

    signal(SIGINT, signalHandler);

    bool isServer = true;

    if (argc > 1)
    {
        printf("Argument 1: %s\n", argv[1]);
        isServer = (strncmp(argv[1], "client", 6) != 0);
    }

    printf("Running as %s\n", isServer ? "server" : "client");

    Shutdown_init();

    if (isServer)
    {
        DFRobotPirate_init();
        Server_init();

        DFRobotPirate_cleanUp();
        Server_cleanup();
    }
    else
    {
        Client_init(argv[2], SERVER_PORT);
        Client_cleanup();
    }

    Shutdown_cleanup();

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