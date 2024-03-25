#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils/time_utils.h"
#include "hal/joystick.h"

#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.1.37"
#define SERVER_PORT 12345
#define MESSAGE_SIZE 1024

//UDP socket
static int sockFD;
static struct sockaddr_in client_addr;
static char send_buffer[MESSAGE_SIZE];

//UDP server thread
static pthread_t clientThreadID;
static bool stopThread = false;
static bool stopApplication = false;


//Helper functions
static void UDP_joinClientThread(void)
{
    if(stopThread)
    {
        //Block the mainThread to Join the readerThread.
        pthread_join(clientThreadID, NULL);
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
    
    memset(&client_addr, '\0', sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(SERVER_PORT);
    client_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    //Bind the socket to the server address.
    if( bind(sockFD, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0)
    {
        printf("Socket bind failed.\n");
        exit(1);
    }

    return;
}

static void UDP_readJS(void)
{
    direction next_move = Joystick_read();
    char* command_name = Joystick_getDirectionString(next_move);
    snprintf(send_buffer,MESSAGE_SIZE, "%s",command_name); 
    return;
}

static void UDP_sendCommand()
{
    //Initialize the send buffer.
    memset(&send_buffer, '\0', MESSAGE_SIZE);

    //Read the Joystick to determine driving direction.
    UDP_readJS();
    
    //Send response to client.
    sendto(sockFD, send_buffer, MESSAGE_SIZE, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    
    return;
}

static void* UDP_clientThreadExecute(void* arg)
{
    //Create the UDP socket.
    UDP_createSocket();

    while(true)
    {
        if(stopThread || stopApplication)
        {
            break;
        }

        //Send a command.
        UDP_sendCommand();
        sleepForMs(30);
    }
    
    //Close the UDP socket.
    close(sockFD);
    return arg;
}


//(1)
void UDP_client_start(void)
{
    pthread_create(&clientThreadID, NULL, UDP_clientThreadExecute, NULL);
    UDP_joinClientThread();
    return;
}

//(2)
void UDP_client_stop(void)
{
    //Make the readerThread exit by setting the stopThread flag. 
    stopThread = true;
    UDP_joinClientThread();
    return;
}

//(3)
bool UDP_client_isTerminated(void)
{
    return stopApplication;
}