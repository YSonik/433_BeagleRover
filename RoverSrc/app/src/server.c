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

        printf("Received message: %s\n", messageRX);

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
                int speed = atoi(&messageRX[6]);
                DFRobotPirate_setSpeed(speed);
            }
            else
            {
                char response[MSG_MAX_LEN];
                sprintf(response, "%d\n", DFRobotPirate_getSpeed());
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
                    DFRobotPirate_moveForward();
                    break;

                case '1':
                    DFRobotPirate_moveBackward();
                    break;

                case '2':
                    DFRobotPirate_turnLeft();
                    break;

                case '3':
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

            sprintf(response, "Accel Response\n");
            // sprintf(response, "%d,%d,%d\n", DFRobotPirate_getAccelX(), DFRobotPirate_getAccelY(), DFRobotPirate_getAccelZ());
            Socket_reply_to_last(response);
        }
        else if (strncmp(messageRX, "gyro", 4) == 0)
        {
            char response[MSG_MAX_LEN];

            sprintf(response, "Gyro Response\n");
            // sprintf(response, "%d,%d,%d\n", DFRobotPirate_getGyroX(), DFRobotPirate_getGyroY(), DFRobotPirate_getGyroZ());
            Socket_reply_to_last(response);
        }else if (strncmp(messageRX, "distance", 8) == 0)
        {
            char response[MSG_MAX_LEN];

            sprintf(response, "Distance Response\n");
            // sprintf(response, "%d\n", DFRobotPirate_getDistance());
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

    Socket_close();

    is_initialized = false;
}