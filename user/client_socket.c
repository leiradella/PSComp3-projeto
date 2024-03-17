#include "client_socket.h"
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define SISMON "/tmp/SISMON"
#define INTUTI "/tmp/INTUTI"

//this creates a socket for the program
void create_client_socket(struct ClientSocket *soc)
{
  strcpy(soc->sisname, SISMON);
  strcpy(soc->usrname, INTUTI);
  unlink(soc->usrname);

 if ((soc->sd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0 ) {
    perror("Erro a criar socket"); exit(-1);
  }
      
  soc->usr.sun_family = AF_UNIX;
  memset(soc->usr.sun_path, 0, sizeof(soc->usr.sun_path));
  strcpy(soc->usr.sun_path, soc->usrname);
  soc->usrlen = (socklen_t)(sizeof(soc->usr.sun_family) + strlen(soc->usr.sun_path));

  if (bind(soc->sd, (struct sockaddr *)&soc->usr, soc->usrlen) < 0 ) {
    perror("Erro no bind"); exit(-1);
  }

  soc->sis.sun_family = AF_UNIX;
  memset(soc->sis.sun_path, 0, sizeof(soc->sis.sun_path));
  strcpy(soc->sis.sun_path, soc->sisname);
  soc->sislen = (socklen_t)(sizeof(soc->sis.sun_family) + strlen(soc->sis.sun_path));

  soc->reg.sun_family = AF_UNIX;
  memset(soc->reg.sun_path, 0, sizeof(soc->reg.sun_path));
  strcpy(soc->reg.sun_path, soc->regname);
  soc->reglen = (socklen_t)(sizeof(soc->reg.sun_family) + strlen(soc->reg.sun_path));
}
