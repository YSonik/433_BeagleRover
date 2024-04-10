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
#include "handshake.h"

#define IP_LEN 50
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
        Server_init();
        Server_cleanup();
    }
    else
    {
        char server_ip[IP_LEN];
        // Client discovers the server.
        memset(server_ip, '\0', IP_LEN);
        Handshake_init(server_ip);

        printf("Server IP address: %s\n", server_ip);
        Client_init(server_ip, SERVER_PORT);
        Client_cleanup();
    }

    Shutdown_cleanup();
    return 0;
}
