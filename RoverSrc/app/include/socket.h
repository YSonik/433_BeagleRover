#ifndef _SOCKET_H_
#define _SOCKET_H_

#define MSG_MAX_LEN 1024
#define SERVER_PORT "12345"
#define CLIENT_PORT "12346"

void Socket_init(const char *l_port,const char *r_ip,const char *r_port);
void Socket_close();

void Socket_send(char *message);
void Socket_reply_to_last(char *message);
void Socket_receive(char *message);

void Socket_get_client_ip(char* client_ip);
void Socket_get_local_ip(char* local_ip);

#endif