#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "socket.h"

static int local_socket;
static int remote_socket;

static struct addrinfo *remote;
static struct sockaddr_in B_addr;

static struct sockaddr client_addr;
static socklen_t client_addr_len = sizeof(client_addr);

static void Socket_init_send_socket(const char *address, const char *port)
{
    int status;
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((status = getaddrinfo(address, port, &hints, &res)) != 0)
    {
        printf("getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    for (remote = res; remote != NULL; remote = remote->ai_next)
    {
        if ((remote_socket = socket(remote->ai_family, remote->ai_socktype,
                                    remote->ai_protocol)) == -1)
        {
            perror("Remote: socket");
            continue;
        }

        break;
    }
}

static void Socket_init_local_socket(const char *port, bool isBroadcast)
{
    int status;
    struct addrinfo hints;
    struct addrinfo *res, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0)
    {
        printf("getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    for (p = res; p != NULL; p = p->ai_next)
    {
        if ((local_socket = socket(p->ai_family, p->ai_socktype,
                                   p->ai_protocol)) == -1)
        {
            perror("Local: socket");
            continue;
        }

        if(isBroadcast)
        {
            //Enable Broadcasting
            int enableBroadcast = 1;
            if(setsockopt(local_socket, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast)) == -1)
            {
                perror("Error enabling socket broadcast\n");
                shutdown(local_socket, SHUT_RDWR);
                exit(1);
            }
        }

        if (bind(local_socket, p->ai_addr, p->ai_addrlen) == -1)
        {
            shutdown(local_socket, SHUT_RDWR);
            perror("Local: bind");
            continue;
        }
        break;
    }

    if(isBroadcast)
    {
        memset(&B_addr, 0, sizeof(B_addr));
        B_addr.sin_family = AF_INET;
        B_addr.sin_port = htons(atoi(port));
        B_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    }

    freeaddrinfo(res); // free the linked list
}

void Socket_init(const char *l_port, const char *r_ip, const char *r_port, bool isBroadcast)
{
    Socket_init_local_socket(l_port, isBroadcast);

    if (r_ip != NULL && r_port != NULL)
    {
        Socket_init_send_socket(r_ip, r_port);
    }
}

void Socket_close()
{
    freeaddrinfo(remote);
    shutdown(local_socket, SHUT_RDWR);
    shutdown(remote_socket, SHUT_RDWR);
}

void Socket_send(char *message, bool isBroadcast)
{
    int bytesSx = 0;
    if(isBroadcast)
    { 
        printf("Sending broadcast:%s\n",message);
        bytesSx = sendto(
        local_socket,
        message,
        strlen(message),
        0,
        (struct sockaddr*)&B_addr,
        sizeof(B_addr));
    }
    else
    {
        bytesSx = sendto(
        remote_socket,
        message,
        strlen(message),
        0,
        remote->ai_addr,
        remote->ai_addrlen);
    }

    if (bytesSx == -1)
    {
        perror("sendto");
        exit(3);
    }
}

void Socket_receive(char *message)
{
    int bytesRX = recvfrom(
        local_socket,
        message,
        MSG_MAX_LEN,
        0,
        &client_addr,
        &client_addr_len);

    if (bytesRX == -1)
    {
        perror("recvfrom");
        exit(3);
    }

    int terminateIdx = (bytesRX < MSG_MAX_LEN) ? bytesRX : MSG_MAX_LEN - 1;
    message[terminateIdx] = '\0';
}

void Socket_reply_to_last(char *message)
{
    int bytesSx = sendto(
        local_socket,
        message,
        strlen(message),
        0,
        remote->ai_addr,
        remote->ai_addrlen);

    if (bytesSx == -1)
    {
        perror("sendto");
        exit(3);
    }
}