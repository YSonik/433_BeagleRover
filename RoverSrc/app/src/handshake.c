#include "handshake.h"
#include "socket.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

void Handshake_init(char* ip_buffer, int ip_len)
{
    Socket_init(CLIENT_PORT, NULL, NULL, true);
    char* broadcast_message = "ip";
    char response[100];

    for(int i=0; i<10; i++)
    {
        Socket_send(broadcast_message);
        memset(response, '\0', 100);
        Socket_receive(response);
        printf("%s\n",response);
    }

    Socket_close();

    return;
}