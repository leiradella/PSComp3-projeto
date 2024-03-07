#include "server_socket.h"
#include "sys/socket.h"
#include <stdlib.h>
#include <stdio.h>

//this creates a socket for the program
void sock_create(serversocket *soc)
{
  strcpy(soc->servname, "/tmp/SERV");

 if ((soc->sd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0 ) {
    perror("Erro a criar socket"); exit(-1);
  }
      
  soc->my_addr.sun_family = AF_UNIX;
  memset(soc->my_addr.sun_path, 0, sizeof(soc->my_addr.sun_path));
  strcpy(soc->my_addr.sun_path, soc->servname);
  soc->addrlen = (socklen_t)(sizeof(soc->my_addr.sun_family) + strlen(soc->my_addr.sun_path));

  if (bind(soc->sd, (struct sockaddr *)&soc->my_addr, soc->addrlen) < 0 ) {
    perror("Erro no bind"); exit(-1);
  }

  soc->fromlen = sizeof(soc->from);
}
