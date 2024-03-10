#ifndef SERVER_COMMANDS_H_
#define SERVER_COMMANDS_H_

#include "server_socket.h"
#include "server_threads.h"

void handle_commands(char *command, serversocket servsock, thinput **threadinput1);

#endif
