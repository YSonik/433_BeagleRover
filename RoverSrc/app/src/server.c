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

        sleepForMs(1000);
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

    Socket_close();

    is_initialized = false;
}