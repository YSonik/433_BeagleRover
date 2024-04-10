#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "handshake.h"
#include "socket.h"

#define MESSAGE_LEN 100

static int client_socket = 0;

static struct sockaddr_in broadcast_address;

char receiveBuffer[MESSAGE_LEN];
char sendBuffer[MESSAGE_LEN];

static void createClientSocket()
{
    // Create the client socket.
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("Client socket could not be created\n");
        exit(1);
    }

    // Enable broadcast.
    int broadcast_enable = 1;
    if (setsockopt(client_socket, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) == -1)
    {
        perror("setsockopt (SO_BROADCAST)");
        exit(1);
    }

    // Initialize the struct for broadcasting address.
    memset(&broadcast_address, 0, sizeof(broadcast_address));
    broadcast_address.sin_family = AF_INET;
    broadcast_address.sin_port = htons(atoi(SERVER_PORT));
    broadcast_address.sin_addr.s_addr = inet_addr("192.168.234.255"); // Broadcasting to all devices in the network

    return;
}

static void clientSend()
{
    // Send the discovery message.
    memset(sendBuffer, '\0', MESSAGE_LEN);
    snprintf(sendBuffer, MESSAGE_LEN, "%s", "ip");
    if (sendto(client_socket, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr *)&broadcast_address, sizeof(broadcast_address)) == -1)
    {
        printf("Client could not send to server\n");
        exit(1);
    }

    // printf("Client broadcasted the following discovery message: %s\n",sendBuffer);

    return;
}

static void clientReceive()
{
    while (true)
    {
        memset(receiveBuffer, '\0', MESSAGE_LEN);
        // socklen_t server_len = sizeof(server_address);

        // Receive discovery message from client.
        ssize_t bytesRx = recvfrom(client_socket, (char *)receiveBuffer, MESSAGE_LEN, 0, NULL, NULL);

        if (bytesRx == -1)
        {
            printf("Client could not receive from server\n");
            exit(1);
        }
        else
        {
            // printf("Client received the following message: %s\n", receiveBuffer);
            break;
        }
    }

    return;
}

void Handshake_init(char *ip_buffer)
{
    // Create the client socket.
    createClientSocket();

    // Send the discovery message.
    clientSend();

    // Listen for the server's response.
    clientReceive();

    // Close the client socket
    close(client_socket);

    // Write the Server's IP address to the passed buffer.
    snprintf(ip_buffer, MESSAGE_LEN, "%s", receiveBuffer);
    return;
}