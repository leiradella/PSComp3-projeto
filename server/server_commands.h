#ifndef SERVER_COMMANDS_H_
#define SERVER_COMMANDS_H_

#include "server_socket.h"
#include "server_threads.h"

// SISMON COMMAND ID DEFINES
#define TSM 3
#define CALA 4
#define CER 5
#define AER 6
#define DER 7
#define CPS 8
#define CTS 9
#define MPPS 10
#define MPPA 11
#define MPPAMB 12
#define DALA 13
#define AAS 14

void tsm(serversocket servsock);
void cala(serversocket servsock);
void cts(serversocket servsock, thinput **threadinput, char** args);
void cps(serversocket servsock, thinput **threadinput, char** args);
void dala(serversocket servsock, char** args);
void cer (serversocket servsock);
void aer(serversocket servsock);
void der(serversocket servsock);
void mpps(serversocket servsock, thinput **threadinput, char** args);
void mppa(serversocket servsock, thinput **threadinput, char** args);
void mppamb(serversocket servsock, thinput **threadinput, char** args);

void handle_commands(char *command, serversocket servsock, thinput **threadinput1);
void handler1(char **args, int argc, thinput **threadinput, serversocket servsock);
void handler2(char **args, int argc, thinput **threadinput, serversocket servsock);

void check_reghist();

#endif
