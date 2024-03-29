#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "handshake.h"
#include "socket.h"

#define MESSAGE_LEN 100
static int server_socket = 0;
static int client_socket = 0;

static struct sockaddr_in server_address;
static struct sockaddr_in client_address;
static struct sockaddr_in broadcast_address;

char receiveBuffer[MESSAGE_LEN];
char sendBuffer[MESSAGE_LEN];

static void createServerSocket()
{
    //Create the server socket.
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("Server socket could not be created\n");
        exit(1);
    }

    //Initialize the server address struct.
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(SERVER_PORT));

    //Bind the socket to the server address struct.
    if (bind(server_socket, (const struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        printf("Server socket bind failed\n");
        exit(1);
    }

    //Enable broadcast.
    int broadcastEnable = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) == -1) {
        printf("Server socket options could not be set\n");
        exit(1);
    }

    return;
}

static void serverListen()
{
    while(true)
    {
        memset(receiveBuffer, '\0', MESSAGE_LEN);  
        socklen_t client_len = sizeof(client_address);

        //Receive discovery message from client.
        ssize_t bytesRx = recvfrom(server_socket, (char *)receiveBuffer, MESSAGE_LEN, 0, (struct sockaddr *)&client_address, &client_len);
        
        if (bytesRx == -1) {
            printf("Server could not receive from client\n");
            exit(1);
        }

        printf("Server received the following message: %s\n", receiveBuffer);
    }

    return;
}


static void createClientSocket()
{
    //Create the client socket.
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("Client socket could not be created\n");
        exit(1);
    }

    //Enable broadcast.
    int broadcast_enable = 1;
    if (setsockopt(client_socket, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        exit(1);
    }

    //Initialize the struct for broadcasting address.
    memset(&broadcast_address, 0, sizeof(broadcast_address));
    broadcast_address.sin_family = AF_INET;
    broadcast_address.sin_port = htons(atoi(SERVER_PORT));
    broadcast_address.sin_addr.s_addr = inet_addr("192.168.234.255"); // Broadcasting to all devices in the network
    
    return;
}

static void clientSend()
{
    //Send the discovery message.
    memset(sendBuffer, '\0', MESSAGE_LEN);
    snprintf(sendBuffer, MESSAGE_LEN, "%s", "ip");
    if (sendto(client_socket, sendBuffer, strlen(sendBuffer), 0, (struct sockaddr*)&broadcast_address, sizeof(broadcast_address)) == -1) {
        printf("Client could not send to server\n");
        exit(1);
    }
    
    printf("Client broadcasted the following discovery message: %s\n",sendBuffer);

    return;
}


void Handshake_init(char* ip_buffer, bool isServer)
{
    if(isServer)
    {
        //Create the server socket.
        createServerSocket();

        //Listen for discovery message. 
        serverListen();

        //Close the server socket.
        close(server_socket);
    }
    else
    {
        //Create the server socket.
        createClientSocket();

        //Listen for discovery message. 
        clientSend();

        //Close the client socket
        close(client_socket);
    }
    
    return;
}