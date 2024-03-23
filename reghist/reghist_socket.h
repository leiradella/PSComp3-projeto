#ifndef REGHIST_SOCKET_H
#define REGHIST_SOCKET_H

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

typedef struct ReghistSocket {
  int sd;
  struct sockaddr_un from; //socket user
  socklen_t fromlen;
  struct sockaddr_un reg; //socket do reg
  socklen_t reglen;
  char regname[20];
} regsocket;

void create_reghist_socket(struct ReghistSocket *soc);

#endif