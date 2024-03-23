#ifndef CLIENT_COMMANDS_H_
#define CLIENT_COMMANDS_H_
#include "client_socket.h"

#define NCOMMANDS (sizeof(commands)/sizeof(struct command_d))

#define HANDLER1 1
#define HANDLER2 2

// INTUTI COMMAND ID DEFINES
#define SAIR 0
#define HELP 1
#define MODE 2

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

// REGHIST COMMAND ID DEFINES
#define LREG 15
#define TRH 16

struct command_d {
  char*	cmd_name;
  char*	cmd_help;
};

void send_command(char *command, clientsocket clisoc);
void handler1(char *message, char *token, char *arg1, int *handler, clientsocket clisoc);
void handler2(char *message, char *token, char *arg1, int *handler, clientsocket clisoc);
void set_mode(char *message, int *handler);

#endif