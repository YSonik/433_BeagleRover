#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "hal/UDP_server.h"

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.38.12" //This needs to be dynamic using getAddr function.
#define SERVER_PORT 12345
#define MESSAGE_SIZE 1024
#define COMMAND_SIZE 20

//UDP socket
static int sockFD;
static struct sockaddr_in server_addr;
static struct sockaddr_in client_addr;
static char receive_buffer[MESSAGE_SIZE];
static char send_buffer[MESSAGE_SIZE];

//UDP server thread
static pthread_t serverThreadID;
static bool stopThread = false;
static bool stopApplication = false;


//Helper functions
static void UDP_joinServerThread(void)
{
    if(stopThread)
    {
        //Block the mainThread to Join the readerThread.
        pthread_join(serverThreadID, NULL);
    }
    return;
}

static void UDP_createSocket(void)
{
    //Create an unbound socket.
    sockFD = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockFD < 0)
    {
        printf("Socket creation failed.\n");
        exit(1);
    }
    
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    //Bind the socket to the server address.
    if( bind(sockFD, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Socket bind failed.\n");
        exit(1);
    }

    return;
}

static void UDP_receiveMessage()
{
    //Initialize the receive buffer.
    memset(&receive_buffer, '\0', MESSAGE_SIZE);
    socklen_t addr_size = sizeof(client_addr);

    //Receive message from client.
    recvfrom(sockFD, receive_buffer, MESSAGE_SIZE, 0, (struct sockaddr*)&client_addr, &addr_size);

    return;
}

static void UDP_sendResponse()
{
    //Initialize the send buffer.
    memset(&send_buffer, '\0', MESSAGE_SIZE);

    //Parse the receive buffer to identify the command.
    //UDP_identifyCommand();

    //Execute the command.
    //UDP_executeCommand();
    
    //Send response to client.
    sendto(sockFD, send_buffer, MESSAGE_SIZE, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    
    return;
}

static void* UDP_serverThreadExecute(void* arg)
{
    //Create the UDP socket.
    UDP_createSocket();

    while(true)
    {
        if(stopThread || stopApplication)
        {
            break;
        }

        //Listen for a message.
        UDP_receiveMessage();

        //Send a response.
        UDP_sendResponse();
        
    }
    
    //Close the UDP socket.
    close(sockFD);
    return arg;
}


//(1)
void UDP_start(void)
{
    pthread_create(&serverThreadID, NULL, UDP_serverThreadExecute, NULL);
    UDP_joinServerThread();
    return;
}

//(2)
void UDP_stop(void)
{
    //Make the readerThread exit by setting the stopThread flag. 
    stopThread = true;
    UDP_joinServerThread();
    return;
}

//(3)
bool UDP_isTerminated(void)
{
    return stopApplication;
}