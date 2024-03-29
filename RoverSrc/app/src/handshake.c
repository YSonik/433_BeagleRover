#include "handshake.h"
#include "socket.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

void Handshake_init(char* ip_buffer, bool isServer)
{
    printf("isServer: %d\n", isServer);
    Socket_init(CLIENT_PORT, NULL, NULL, true);
    char* broadcast_message = "ip";

    for(int i=0; i<10; i++)
    {
        Socket_send(broadcast_message);
        Socket_receive(ip_buffer);
        printf("%s\n",ip_buffer);
    }

    Socket_close();

    return;
}