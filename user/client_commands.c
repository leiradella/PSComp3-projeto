#include "client_commands.h"
#include "client_socket.h"
#include <stdio.h>
#include <string.h>

const struct command_d commands[] = {
  {"sos",              "                    -help"},
  {"help",             "                    -help"},
  {"sair",             "                    -sair da interface do usuario"},
  {"modo m",           "                    -define o modo como a mensagem e enviada ao server (1 = command_id, 2 = command string)"},
  {"tsm",              "                    -terminar servidor"},
  {"trh",              "                    -terminar registo"},
  {"cts s",            "                    -cosnulta temperatura do setor s (0 = todos setores)"},
  {"cps s",            "                    -consulta os parametros do setor s (0 = todos setores)"},
  {"mpps s p",         "                    -modifica periodo dos sensores do setor s (0 = todos setores) para p (novo periodo)"},
  {"mppa s p",         "                    -modifica periodo dos actuadores do setor s (0 = todos setores) para p (novo periodo)"},
  {"mppamb s p",       "                    -modifica periodo do ambiente do setor s (0 = todos setores) para p (novo periodo)"},
  {"dala m M  ",       "                    -modifica os limites de temperatura de alarme para todos setores (m = temperatura minima M = temperatura maxima)"},
  {"cala",             "                    -consulta as temperaturas de alarma dos setores"},
  {"aas s v",          "                    -ativa atuador do setor s (0 = todos setores) com variação v (-+0)"},
  {"cer",              "                    -consultar estado do envio dos registos para o histórico"},
  {"aer",              "                    -ativa o envio de registos para o histórico"},
  {"der",              "                    -desativa o envio de registos para o histórico"},
  {"lreg s [t1 [t2]]", "                    -lista registosdo setor s (0 = todos setores) entre t1 e t2, caso t2 nao seja especificado, lista todos desde t1 até o presente"}
};


void send_command(char *command, clientsocket clisoc)
{
    char *token = NULL;
    char *message = NULL;
    char *arg1;
    // this variable decides the format of the final message
    //handler1 returns message with format "command_id argument argument argument"
    //handler2 returns message with format "command argument argument argument"
    //only difference is that handler 1 will send just numbers, but cant handle commands with strings as arguments
    static int handler = HANDLER1;

    if (strcmp(command, "\n") == 0)
    {
        return;
    }

    //remove \n from the string
    command[strlen(command)-1] = '\0';

    //check if message is just spaces
    token = strtok(command, " ");
    if (token == NULL)
    {
        return;
    }

    //get space for arg1
    arg1 = (char *)malloc(sizeof(command)*sizeof(char));
    if (arg1 == NULL){
      printf("Erro ao alocar memoria\n");
      return;
    }

    //get space for message
    message = (char *)malloc(sizeof(command)*sizeof(char)+2);
    if (message == NULL){
      printf("Erro ao alocar memoria\n");
      return;
    }

    //clean message
    strcpy(message, "");

    if (handler == HANDLER1) handler1(message, token, arg1, &handler, clisoc);
    else if (handler == HANDLER2) handler2(message, token, arg1, &handler, clisoc);

    free(message);
    free(arg1);
}

//this function sends message to the server as "command_id argument argument ... argument"
void handler1(char *message, char *token, char *arg1, int *handler,clientsocket clisoc)
{
    //char *temp;
    int id, i, argc = 0;

    //get the fist word (command) and put it in arg1
    strcpy(arg1, token);

    // i hate myself
    if (strcmp(arg1, "sair") == 0){
        id = SAIR;
    } else if (strcmp(arg1, "sos") == 0 || strcmp(arg1, "help") == 0) {
        id = HELP;
    } else if (strcmp(arg1, "tsm") == 0) {
        id = TSM; 
    } else if (strcmp(arg1, "modo") == 0) {
        id = MODO;
    } else if (strcmp(arg1, "cala") == 0) {
        id = CALA;
    } else if (strcmp(arg1, "cer") == 0) {
        id = CER;
    } else if (strcmp(arg1, "aer") == 0) {
        id = AER;
    } else if (strcmp(arg1, "der") == 0) {
        id = DER;
    } else if (strcmp(arg1, "cps") == 0) {
        id = CPS;
    } else if (strcmp(arg1, "cts") == 0) {
        id = CTS;
    } else if (strcmp(arg1, "mpps") == 0) {
        id = MPPS;
    } else if (strcmp(arg1, "mppa") == 0) {
        id = MPPA;
    } else if (strcmp(arg1, "mppamb") == 0) {
        id = MPPAMB;
    } else if (strcmp(arg1, "dala") == 0) {
        id = DALA;
    } else if (strcmp(arg1, "aas") == 0) {
        id = AAS;
    } else if (strcmp(arg1, "lreg") == 0) {
        id = LREG;
    } else if (strcmp(arg1, "trh") == 0) {
        id = TRH;
    }

    //store the id in message and pass it onto arg1
    snprintf(message, 4, "%d", id);
    strcpy(arg1, message);
    
    //1st cycle because of the space lol
    token = strtok(NULL, " ");
    argc++;

    //this is to make the message format standard. for example if command is "dala      2   3     " message will be "13 2 3".         
    while(token != NULL)
    {
        argc++;
        strcat(message, " ");
        strcat(message, token);
        token = strtok(NULL, " ");
    }

    switch (argc) {
        case 1:
            switch (id) {
                case SAIR:
                    printf("Client closing...\n");
                    free(message);
                    free(arg1);
                    exit(0);
                case HELP:
                   for(i = 0; i < (int)NCOMMANDS; i++)
                    {
                        printf("%s %s\n", commands[i].cmd_name, commands[i].cmd_help);
                    }
                    break;
                case TRH:
                    if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.reg, clisoc.reglen) < 0) perror("CLI: Erro no sendto");
                    break;
                case TSM:
                case CALA:
                case CER:
                case AER:
                case DER:
                    if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                    break;
                default:
                    printf("Intuti: comando invalido (argc = %d)\n", argc);
                    break;
            }
            break;
        case 2: 
            switch (id) {
                case MODO:
                    set_mode(message, handler);
                    break; 
                case CPS:
                case CTS:
                    if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                    break;
                default:
                    printf("Intuti: comando invalido (argc = %d)\n", argc);
                    break;
            }
            break;
        case 3: switch (id) {
            case MPPS:
            case MPPA:
            case MPPAMB:
            case DALA:
            case AAS:
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            case LREG:
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.reg, clisoc.reglen) < 0) perror("CLI: Erro no sendto");
                break;
            default:
                printf("Intuti: comando invalido (argc = %d)\n", argc);
                break;
            }
            break;
        case 4: switch (id) {
            case LREG:
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.reg, clisoc.reglen) < 0) perror("CLI: Erro no sendto");
                break;
            default:
                printf("Intuti: comando invalido (argc = %d)\n", argc);
                break;
            }
            break;
        default:
            printf("Intuti: comando invalido (argc = %d)\n", argc);
            break; 
    }

}

//this function sends message to the server as "command argument argument ... argument"
void handler2(char *message, char *token, char *arg1, int *handler, clientsocket clisoc)
{
    int i, argc = 0;

    //get the fist word (command) and put it in message and arg1
    strcpy(message, token);
    strcpy(arg1, token);

    //1st cycle because of the space lol
    token = strtok(NULL, " ");
    argc++;

    //this is to make the message format standard. for example if command is "dala      2   3     " message will be "dala 2 3".         
    while(token != NULL)
    {
        argc++;
        strcat(message, " ");
        strcat(message, token);
        token = strtok(NULL, " ");
    }

    switch (argc) {
        case 1:
            if(strcmp(arg1, "tsm") == 0 || strcmp(arg1, "cala") == 0 || strcmp(arg1, "cer") == 0 || strcmp(arg1, "aer") == 0 || strcmp(arg1, "der") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            else if(strcmp(arg1, "sair") == 0)
            {
                printf("Client closing...\n");
                free(message);
                free(arg1);
                exit(0);        
            }
            else if(strcmp(arg1, "sos") == 0 || strcmp(arg1, "help") == 0)
            {
                for(i = 0; i < (int)NCOMMANDS; i++)
                {
                    printf("%s %s\n", commands[i].cmd_name, commands[i].cmd_help);
                }
                break;
            }
            else if(strcmp(arg1, "trh") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.reg, clisoc.reglen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            printf("Intuti: comando invalido (argc = %d)\n", argc);
            break;
        case 2:
            if(strcmp(arg1, "modo") == 0)
            {
                set_mode(message, handler);
                break;
            }
            else if(strcmp(arg1, "cps") == 0 || strcmp(arg1, "cts") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            printf("Intuti: comando invalido (argc = %d)\n", argc);
            break;
        case 3:
            if(strcmp(arg1, "mpps") == 0 || strcmp(arg1, "mppa") == 0 || strcmp(arg1, "mppamb") == 0 || strcmp(arg1, "dala") == 0 || strcmp(arg1, "aas") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            else if (strcmp(arg1, "lreg") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.reg, clisoc.reglen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            printf("Intuti: comando invalido (argc = %d)\n", argc);
            break;
        case 4:
            if (strcmp(arg1, "lreg") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.reg, clisoc.reglen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            printf("Intuti: comando invalido (argc = %d)\n", argc);
            break;
        default:
            printf("Intuti: comando invalido (argc = %d)\n", argc);
            break;
    }
}

void set_mode(char *message, int *handler)
{
    char *end, *token;
    int num;

    token = strtok(message, " "); //get 1st word (command)
    token = strtok(NULL, " "); //get 2nd word (value for handler)

    num = (int)strtol(token, &end, 10);

    if (num != 1 && num != 2)
    {
        printf("Intuti: Invalid mode: %d\n", num);
        return;
    }

    if (num == (*handler))
    {
        printf("modo %d ja esta ativo\n", num);
    } else {
        (*handler) = num;
        printf("modo %d ativo\n", num);
    }
}
