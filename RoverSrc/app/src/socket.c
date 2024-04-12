#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "socket.h"

static int local_socket;
static int remote_socket;

static struct addrinfo *remote;

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

static void Socket_init_local_socket(const char *port)
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

        if (bind(local_socket, p->ai_addr, p->ai_addrlen) == -1)
        {
            shutdown(local_socket, SHUT_RDWR);
            perror("Local: bind");
            continue;
        }
        break;

        // struct timeval tv;
        // tv.tv_sec = 5;
        // tv.tv_usec = 0;
        // if (setsockopt(local_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv) == -1)
        // {
        //     perror("setsockopt");
        //     exit(1);
        // }
    }

    freeaddrinfo(res); // free the linked list
}

void Socket_init(const char *l_port, const char *r_ip, const char *r_port)
{
    Socket_init_local_socket(l_port);

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

void Socket_send(char *message)
{
    int bytesSx = 0;
    bytesSx = sendto(
        remote_socket,
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
    const struct sockaddr *client_sockaddr = &client_addr;

    int bytesSx = sendto(
        local_socket,
        message,
        strlen(message),
        0,
        client_sockaddr,
        client_addr_len);

    if (bytesSx == -1)
    {
        perror("sendto");
        exit(3);
    }
}

void Socket_get_client_ip(char *client_ip)
{
    struct sockaddr_in *client_addr_in = (struct sockaddr_in *)&client_addr;
    inet_ntop(AF_INET, &client_addr_in->sin_addr, client_ip, INET_ADDRSTRLEN);
}

void Socket_get_local_ip(char *local_ip)
{
    // https://stackoverflow.com/questions/2283494/get-ip-address-of-an-interface-on-linux

    struct ifreq ifr;

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ - 1);

    ioctl(local_socket, SIOCGIFADDR, &ifr);

    sprintf(local_ip, "%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
}