/***************************************************************************
| File:   server_socket.h
|
| Group:  4
|
| Autors: Miguel Fernades  103024
|         Gonçalo Antunes  103524
|         Lucas Leiradella 103566
|
| Data:  Mar 2024
***************************************************************************/
#ifndef SERVER_SOCKET_H_
#define SERVER_SOCKET_H_

#include <sys/un.h>
#include <sys/socket.h>

#define MAXWORDS 50

struct Message {
  char *argv[MAXWORDS];
  int argc;
};

typedef struct ServerSocket {
  int sd;
  struct sockaddr_un from; //socket user
  socklen_t fromlen;
  struct sockaddr_un sis; //socket do sismon
  socklen_t sislen;
  char sisname[20];
} serversocket;

void sock_create(serversocket *soc);
#endif
