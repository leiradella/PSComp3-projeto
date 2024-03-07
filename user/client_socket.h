#ifndef CLIENT_SOCKET_H_
#define CLIENT_SOCKET_H_

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>


#define SERVNAME "/tmp/SERV"
#define CLINAME "/tmp/CLI"


extern int sd;
extern struct sockaddr_un my_addr;
extern socklen_t addrlen;
extern struct sockaddr_un to;
extern socklen_t tolen;
extern char buf[100];
  
void create_client_socket ();  
  
 #endif
