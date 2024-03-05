#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

typedef struct ServerSocket {
  int sd;
  struct sockaddr_un my_addr;
  socklen_t addrlen;
  struct sockaddr_un from;
  socklen_t fromlen;
  char servname[24];
} serversocket;

void sock_create(struct ServerSocket *soc);

void change_servname(char* servname);

#endif
