/***************************************************************************
| File:   reghist_commands.h
|
| Group:  4
|
| Autors: Miguel Fernades  103024
|         Gonçalo Antunes  103524
|         Lucas Leiradella 103566
|
| Data:  Mar 2024
***************************************************************************/
#ifndef REGHIST_COMMANDS_H
#define REGHIST_COMMANDS_H

#include "reghist_socket.h"
#include "reghist.h"

// REGHIST COMMAND ID DEFINES
#define LREG 15
#define TRH 16

void trh(regsocket soc);
void lreg(reg_t *pa, char **args, int argc, regsocket soc);

void handle_commands(reg_t *pa, char* command, regsocket soc);
void handler1(reg_t *pa, char **args, int id, int argc,regsocket soc);
void handler2(reg_t *pa, char **args, int argc, regsocket soc);

time_t date_to_seconds(char *date, char *time);
time_t timespec_to_seconds(struct timespec spec);
struct tm *seconds_to_date(struct timespec tspec);

int get_last_valid_id(reg_t *pa);

#endif