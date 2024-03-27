/***************************************************************************
| File:   client_socket.c
|
| Group:  4
|
| Autors: Miguel Fernades  103024
|         Gonçalo Antunes  103524
|         Lucas Leiradella 103566
|
| Data:  Mar 2024
***************************************************************************/
#include "client_socket.h"
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define SISMON "/tmp/SISMON"
#define REGHIST "/tmp/reghist"

//this creates a socket for the user, and also stores the names of sismon and reghist sockets for communication purposes
void create_client_socket(struct ClientSocket *soc)
{
  char INTUTI[50];
  int pid;

  //intuti
  pid = getpid();
  sprintf(INTUTI, "/tmp/INTUTI%i", pid);
  strcpy(soc->sisname, SISMON);
  strcpy(soc->usrname, INTUTI);
  strcpy(soc->regname, REGHIST);
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

  //sismon
  soc->sis.sun_family = AF_UNIX;
  memset(soc->sis.sun_path, 0, sizeof(soc->sis.sun_path));
  strcpy(soc->sis.sun_path, soc->sisname);
  soc->sislen = (socklen_t)(sizeof(soc->sis.sun_family) + strlen(soc->sis.sun_path));

  //reghist
  soc->reg.sun_family = AF_UNIX;
  memset(soc->reg.sun_path, 0, sizeof(soc->reg.sun_path));
  strcpy(soc->reg.sun_path, soc->regname);
  soc->reglen = (socklen_t)(sizeof(soc->reg.sun_family) + strlen(soc->reg.sun_path));
}
