#ifndef REGHIST_COMMANDS_H
#define REGHIST_COMMANDS_H

#include "reghist_socket.h"
#include "reghist.h"

// REGHIST COMMAND ID DEFINES
#define LREG 15
#define TRH 16

void trh(regsocket soc);
void lreg(char *pa, reg_t registo, char **args, int argc, regsocket soc);

void handle_commands(char *pa, reg_t registo, char* command, regsocket soc);
void handler1(char *pa, reg_t registo, char **args, int id, int argc,regsocket soc);
void handler2(char *pa, reg_t registo, char **args, int argc, regsocket soc);

#endif