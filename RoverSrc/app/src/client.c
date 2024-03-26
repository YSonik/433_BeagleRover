#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#include "hal/joystick.h"
#include "utils/time_utils.h"

#include "socket.h"
#include "shutdown.h"
#include "client.h"

static bool is_initialized = false;
static pthread_t client_thread = 0;

static void *Client_thread()
{
    char messageRX[MSG_MAX_LEN];

    while (!Shutdown_isRequested())
    {
        direction direction = Joystick_read();

        printf("Direction: %d\n", direction);

        char directionStr[MSG_MAX_LEN];
        sprintf(directionStr, "%d", direction);

        const char *directionName = Joystick_getDirectionString(direction);
        printf("Sending direction: %s\n", directionName);

        Socket_send(directionStr);

        sleepForMs(1000);
    }

    return NULL;
}

void Client_init(const char *r_ip, const char *r_port)
{
    if (is_initialized)
    {
        return;
    }

    Socket_init(CLIENT_PORT, r_ip, r_port);
    Joystick_init();

    pthread_create(&client_thread, NULL, Client_thread, NULL);

    is_initialized = true;
}

void Client_cleanup()
{
    if (!is_initialized)
    {
        return;
    }

    pthread_join(client_thread, NULL);

    Socket_close();
    Joystick_cleanup();

    is_initialized = false;
}