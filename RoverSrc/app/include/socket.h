#ifndef _SOCKET_H_
#define _SOCKET_H_

#define MSG_MAX_LEN 1024
#define SERVER_PORT "12345"
#define CLIENT_PORT "12346"

void Socket_init(const char *l_port,const char *r_ip,const char *r_port);
void Socket_close();

void Socket_send(char *messageRX);
void Socket_reply_to_last(char *messageRX);
void Socket_receive(char *messageRX);

#endif