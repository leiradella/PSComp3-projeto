#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

typedef struct ClientSocket {
  int sd;
  struct sockaddr_un my_addr; //socket cliente
  socklen_t addrlen;
  struct sockaddr_un to; //socket do servidor
  socklen_t tolen;
  char servname[10]; 
  char cliname[10];
} clientsocket;

void create_client_socket(struct ClientSocket *soc);

#endif
