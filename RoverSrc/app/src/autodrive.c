#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#include "shutdown.h"

#include "hal/distance_sensor.h"
#include "hal/dfrobot_pirate.h"

#include "autodrive.h"

#define MAX_SPEED 100
#define MIN_SPEED 0

static bool is_initialized = false;
static bool is_running = false;
static int speedBeforeStop = 0;

static pthread_t auto_drive_thread = 0;

static void *AutoDrive_thread()
{
    while (!Shutdown_isRequested())
    {
        if (DistanceSensor_getDistance() < 10)
        {
            speedBeforeStop = DFRobotPirate_getSpeed();

            AutoDrive_stop();

            if (!is_running)
            {
                continue;
            }

            int direction = rand() % 2;

            while (DistanceSensor_getDistance() < 10)
            {
                if (direction == 0)
                {
                    DFRobotPirate_turnLeft();
                }else{
                    DFRobotPirate_turnRight();
                }

                DFRobotPirate_setSpeed(speedBeforeStop);
            }

            DFRobotPirate_moveForward();
        }
    }

    return NULL;
}

void AutoDrive_init()
{
    assert(!is_initialized);

    is_initialized = true;

    pthread_create(&auto_drive_thread, NULL, AutoDrive_thread, NULL);
}

void AutoDrive_start()
{
    assert(is_initialized);

    if (!is_running)
    {
        is_running = true;
    }
}

void AutoDrive_stop()
{
    assert(is_initialized);

    if (is_running)
    {
        is_running = false;
    }
}

void AutoDrive_cleanup()
{
    assert(is_initialized);

    is_initialized = false;

    pthread_join(auto_drive_thread, NULL);
}

bool AutoDrive_isRunning()
{
    return is_running;
}