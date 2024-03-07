/***************************************************************************
| File: comando.c  -  Concretizacao de comandos (exemplo)
|
| Autor: Carlos Almeida (IST)
| Data:  Nov 2002
***************************************************************************/
#include "client_socket.h"
#include <stdio.h>
#include <stdlib.h>

#define MSG "YO WORK BOY PLEASE"

/*-------------------------------------------------------------------------+
| Function: cmd_sair - termina a aplicacao
+--------------------------------------------------------------------------*/ 
void cmd_sair (int argc, char **argv)
{
  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_cts (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_cps (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_mpps (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_mppa (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_mppamb (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_aas (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_cala (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_dala (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_cer (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_aer (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_der (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_tsm (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_lreg (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_trh (int argc, char** argv)
{
  if (sendto(sd, MSG, strlen(MSG)+1, 0, (struct sockaddr *)&to, 
	     tolen) < 0) {
    perror("CLI: Erro no sendto");
  }
}


