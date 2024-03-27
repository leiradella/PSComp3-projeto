/***************************************************************************
| File:   server_socket.c
|
| Group:  4
|
| Autors: Miguel Fernades  103024
|         Gonçalo Antunes  103524
|         Lucas Leiradella 103566
|
| Data:  Mar 2024
***************************************************************************/
#include "server_socket.h"
#include "sys/socket.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define SISMON "/tmp/SISMON"

//this creates a socket for the program
void sock_create(serversocket *soc)
{
  strcpy(soc->sisname, SISMON);
  unlink(soc->sisname);

 if ((soc->sd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0 ) {
    perror("Erro a criar socket"); exit(-1);
  }
      
  soc->sis.sun_family = AF_UNIX;
  memset(soc->sis.sun_path, 0, sizeof(soc->sis.sun_path));
  strcpy(soc->sis.sun_path, soc->sisname);
  soc->sislen = (socklen_t)(sizeof(soc->sis.sun_family) + strlen(soc->sis.sun_path));

  if (bind(soc->sd, (struct sockaddr *)&soc->sis, soc->sislen) < 0 ) {
    perror("Erro no bind"); exit(-1);
  }

  soc->fromlen = sizeof(soc->from);
}
