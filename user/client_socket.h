#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

typedef struct ClientSocket {
  int sd;
  struct sockaddr_un usr; //socket user
  socklen_t usrlen;
  char usrname[20];
  struct sockaddr_un sis; //socket do sismon
  socklen_t sislen;
  char sisname[20];
  struct sockaddr_un reg; //socket do reghist
  socklen_t reglen;
  char regname[20]; 

} clientsocket;

void create_client_socket(struct ClientSocket *soc);

#endif
