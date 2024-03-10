#include "client_socket.h"
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

//this creates a socket for the program
void create_client_socket(struct ClientSocket *soc)
{
  strcpy(soc->servname, "/tmp/SERV");
  strcpy(soc->cliname, "/tmp/CLI");
  unlink(soc->cliname);

 if ((soc->sd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0 ) {
    perror("Erro a criar socket"); exit(-1);
  }
      
  soc->my_addr.sun_family = AF_UNIX;
  memset(soc->my_addr.sun_path, 0, sizeof(soc->my_addr.sun_path));
  strcpy(soc->my_addr.sun_path, soc->cliname);
  soc->addrlen = (socklen_t)(sizeof(soc->my_addr.sun_family) + strlen(soc->my_addr.sun_path));

  if (bind(soc->sd, (struct sockaddr *)&soc->my_addr, soc->addrlen) < 0 ) {
    perror("Erro no bind"); exit(-1);
  }

  soc->to.sun_family = AF_UNIX;
  memset(soc->to.sun_path, 0, sizeof(soc->to.sun_path));
  strcpy(soc->to.sun_path, soc->servname);
  soc->tolen = (socklen_t)(sizeof(soc->my_addr.sun_family) + strlen(soc->to.sun_path));
}
