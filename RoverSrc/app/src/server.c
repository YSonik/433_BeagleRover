#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#include "utils/time_utils.h"

#include "hal/accelerometer.h"
#include "hal/distance_sensor.h"
#include "hal/gyroscope.h"
#include "hal/dfrobot_pirate.h"

#include "socket.h"
#include "shutdown.h"

#include "server.h"

static bool is_initialized = false;
static pthread_t server_thread = 0;

static void *Server_thread()
{
    while (!Shutdown_isRequested())
    {
        char messageRX[MSG_MAX_LEN];

        Socket_receive(messageRX);
        /**
         * Supported commands
         * speed=[speed]    Set Speed (0 <= Speed <= 100)
         * speed            Get Speed
         *
         * direction=0      Set Direction
         * direction        Get Direction (0 = Forward, 1 = Backward, 2 = Left, 3 = Right, 4 = Stop)
         *
         * accel            get Accelerometer values
         * gyro             get Gyroscope values
         * distance         get distance from ultrasonic sensor
         *
         * help             get help
         * ip               get IP address
         * shutdown         Shutdown the server
         */

        if (strncmp(messageRX, "speed", 5) == 0)
        {
            if (messageRX[5] == '=')
            {
                printf("Received message: %s", messageRX);
                int speed = atoi(&messageRX[6]);
                DFRobotPirate_setSpeed(speed);
            }
            else
            {
                char response[MSG_MAX_LEN];
                sprintf(response, "%d\n", DFRobotPirate_getSpeed());

                printf("Speed Response: %s", response);

                Socket_reply_to_last(response);
            }
        }
        else if (strncmp(messageRX, "direction", 9) == 0)
        {

            if (messageRX[9] == '=')
            {
                switch (messageRX[10])
                {
                case '0':
                    printf("Received message: %s\n", messageRX);
                    DFRobotPirate_moveForward();
                    break;

                case '1':
                    printf("Received message: %s\n", messageRX);
                    DFRobotPirate_moveBackward();
                    break;

                case '2':
                    printf("Received message: %s\n", messageRX);
                    DFRobotPirate_turnLeft();
                    break;

                case '3':
                    printf("Received message: %s\n", messageRX);
                    DFRobotPirate_turnRight();
                    break;

                case '4':
                case '5':
                    DFRobotPirate_stop();
                    break;

                default:
                    printf("Unknown direction: %c\n", messageRX[10]);
                    break;
                }
            }
        }
        else if (strncmp(messageRX, "accel", 5) == 0)
        {
            char response[MSG_MAX_LEN];

            double xAcc, yAcc, zAcc;
            Accelerometer_readAll(&xAcc, &yAcc, &zAcc);

            snprintf(response, MSG_MAX_LEN, "%.5f %.5f %.5f\n", xAcc, yAcc, zAcc);

            Socket_reply_to_last(response);
        }
        else if (strncmp(messageRX, "gyro", 4) == 0)
        {
            char response[MSG_MAX_LEN];

            int16_t zGy = 0;
            Gyroscope_getAngle(&zGy);
            snprintf(response, 10, "%d", zGy);
            Socket_reply_to_last(response);
        }
        else if (strncmp(messageRX, "distance", 8) == 0)
        {
            char response[MSG_MAX_LEN];

            double distance = DistanceSensor_getDistance();

            snprintf(response, MSG_MAX_LEN, "%.5f\n", distance);

            Socket_reply_to_last(response);
        }
        else if (strncmp(messageRX, "help", 4) == 0)
        {
            char response[MSG_MAX_LEN];
            sprintf(response, "Supported commands:\n"
                              "speed=[speed]    Set Speed (0 <= Speed <= 100)\n"
                              "speed            Get Speed\n"
                              "direction=0      Set Direction\n"
                              "direction        Get Direction (0 = Forward, 1 = Backward, 2 = Left, 3 = Right, 4 = Stop)\n"
                              "accel            get Accelerometer values\n"
                              "gyro             get Gyroscope values\n"
                              "distance         get distance from ultrasonic sensor\n"
                              "help             get help\n"
                              "ip               get IP address\n"
                              "shutdown         Shutdown the server\n");
            Socket_reply_to_last(response);
        }
        else if (strncmp(messageRX, "ip", 2) == 0)
        {
            printf("IP command received\n");

            char response[16];
            memset(response, 0, 16);
            Socket_get_local_ip(response);

            Socket_reply_to_last(response);
        }
        else if (strncmp(messageRX, "shutdown", 8) == 0)
        {
            Shutdown_request();
        }
        else
        {
            printf("Unknown command: %s\n", messageRX);
        }
    }

    return NULL;
}

void Server_init()
{
    if (is_initialized)
    {
        return;
    }

    Socket_init(SERVER_PORT, NULL, NULL);

    DFRobotPirate_init();

    Gyroscope_init();
    DistanceSensor_init();
    // Accelerometer_init();

    pthread_create(&server_thread, NULL, Server_thread, NULL);

    is_initialized = true;
}

void Server_cleanup()
{
    if (!is_initialized)
    {
        return;
    }

    pthread_join(server_thread, NULL);

    DFRobotPirate_cleanup();
    Gyroscope_cleanUp();
    DistanceSensor_cleanup();
    // Accelerometer_cleanup();


    Socket_close();

    is_initialized = false;
}