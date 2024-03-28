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

#define DIRECTION_LEN 2

static bool is_initialized = false;
static pthread_t client_thread = 0;

static void *Client_thread()
{
    while (!Shutdown_isRequested())
    {
        direction direction = Joystick_read();

        char directionStr[DIRECTION_LEN];
        memset(directionStr, '\0', DIRECTION_LEN);
        snprintf(directionStr, 2, "%d", direction);

        const char *directionName = Joystick_getDirectionString(direction);
        printf("Sending direction: %s\n", directionName);

        Socket_send(directionStr, false);

        sleepForMs(10);
    }

    return NULL;
}

void Client_init(const char *r_ip, const char *r_port)
{
    if (is_initialized)
    {
        return;
    }

    Socket_init(CLIENT_PORT, r_ip, r_port, false);
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