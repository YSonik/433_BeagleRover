#include "handshake.h"
#include "socket.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

#define MESSAGE_LEN 100

void Handshake_init(char* ip_buffer, bool isServer)
{
    
    char receiveBuffer[MESSAGE_LEN];
    char sendBuffer[MESSAGE_LEN];
    memset(receiveBuffer, '\0', MESSAGE_LEN);
    memset(sendBuffer, '\0', MESSAGE_LEN);

    if(isServer)
    {
        //Receive discovery broadcast from client.
        Socket_init(SERVER_PORT, NULL, CLIENT_PORT, true);
        Socket_receive(receiveBuffer);
        printf("Server received broadcast: %s\n",receiveBuffer);

        //Respond with server ip.
        snprintf(sendBuffer, MESSAGE_LEN, "%s", "192.168.234.12");
        Socket_reply_to_last(sendBuffer);
    }
    else
    {
        //Send discovery broadcast to server.
        Socket_init(CLIENT_PORT, NULL, SERVER_PORT, true);
        snprintf(sendBuffer, MESSAGE_LEN, "%s", "ip");
        Socket_send(sendBuffer);

        //Listen for server's response.
        Socket_receive(receiveBuffer);
        printf("Client received response: %s\n",receiveBuffer);
    }
    
    Socket_close();

    return;
}