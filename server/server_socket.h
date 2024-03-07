#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

typedef struct ServerSocket {
  int sd;
  struct sockaddr_un my_addr; //socket do servidor
  socklen_t addrlen;
  struct sockaddr_un from; //socket do usuario 
  socklen_t fromlen;
  char servname[10]; 
} serversocket;

void sock_create(struct ServerSocket *soc);
#endif
