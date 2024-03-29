#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "handshake.h"
#include "socket.h"


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
        //Socket_init(SERVER_PORT, NULL, CLIENT_PORT, true);
        //Socket_receive(receiveBuffer);
        //printf("Server received broadcast: %s\n",receiveBuffer);

        //Respond with server ip.
        //snprintf(sendBuffer, MESSAGE_LEN, "%s", "192.168.234.12");
        //Socket_reply_to_last(sendBuffer);
        int sockfd;
        struct sockaddr_in servaddr, cliaddr;
        char buffer[MESSAGE_LEN];

        // Create UDP socket
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        // Initialize server address structure
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on all network interfaces
        servaddr.sin_port = htons(SERVER_PORT);

        // Bind socket
        if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        // Enable broadcast
        int broadcastEnable = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) == -1) {
            perror("setsockopt (SO_BROADCAST)");
            exit(EXIT_FAILURE);
        }

        printf("UDP server listening on port %d...\n", SERVER_PORT);

        while (1) {
            socklen_t len = sizeof(cliaddr);

            // Receive data from client
            ssize_t n = recvfrom(sockfd, (char *)buffer, MESSAGE_LEN, 0,
                                (struct sockaddr *)&cliaddr, &len);
            if (n == -1) {
                perror("recvfrom failed");
                exit(EXIT_FAILURE);
            }

            // Null terminate received data
            buffer[n] = '\0';

            // Display received message
            printf("Received message from %s:%d: %s\n",
                inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buffer);
        }

        // Close socket
        close(sockfd);
    }
    else
    {
        //Send discovery broadcast to server.
        //Socket_init(CLIENT_PORT, NULL, SERVER_PORT, true);
        //snprintf(sendBuffer, MESSAGE_LEN, "%s", "ip");
        //Socket_send(sendBuffer);

        //Listen for server's response.
        //Socket_receive(receiveBuffer);
        //printf("Client received response: %s\n",receiveBuffer);


        int sockfd;
        struct sockaddr_in broadcast_addr;
        char buffer[MESSAGE_LEN] = "Hello, broadcast world!";

        // Create UDP socket
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        // Enable broadcast
        int broadcast_enable = 1;
        if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) == -1) {
            perror("setsockopt (SO_BROADCAST)");
            exit(1);
        }

        // Set up the destination address for broadcasting
        memset(&broadcast_addr, 0, sizeof(broadcast_addr));
        broadcast_addr.sin_family = AF_INET;
        broadcast_addr.sin_port = htons(SERVER_PORT);
        broadcast_addr.sin_addr.s_addr = inet_addr("255.255.255.255"); // Broadcasting to all devices in the network

        // Send the data
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr)) == -1) {
            perror("sendto");
            exit(1);
        }

        printf("Message sent to broadcast address.\n");

        close(sockfd); // Close the socket
        return;
    }
    
    //Socket_close();
    //return;
}