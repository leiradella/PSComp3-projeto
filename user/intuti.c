#include "client_socket.h"
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define MAXMESSAGESIZE 200
#define NCOMMANDS (sizeof(commands)/sizeof(struct command_d))

void *thread_func(void *clisoc)
{
    char buf[MAXMESSAGESIZE];
    clientsocket *soc;
    soc = (clientsocket *)clisoc;
    while(1)
    {
        if (recvfrom(soc->sd, buf, MAXMESSAGESIZE, 0, NULL, NULL) < 0) {
            perror("Erro no recvfrom");
        }
        printf("%s\n", buf);
    }
}

struct 	command_d {
  char*	cmd_name;
  char*	cmd_help;
} const commands[] = {
  {"sos",              "                    -help"},
  {"help",             "                    -help"},
  {"sair",             "                    -sair da interface do usuario"},
  {"tsm",              "                    -terminar servidor"},
  {"cts s",            "                    -cosnulta temperatura do setor s (0 = todos setores)"},
  {"cps s",            "                    -consulta os parametros do setor s (0 = todos setores)"},
  {"mpps s p",         "                    -modifica periodo dos sensores do setor s (0 = todos setores) para p (novo periodo)"},
  {"mppa s p",         "                    -modifica periodo dos actuadores do setor s (0 = todos setores) para p (novo periodo)"},
  {"mppamb s p",       "                    -modifica periodo do ambiente do setor s (0 = todos setores) para p (novo periodo)"},
  {"dala m M  ",       "                    -modifica os limites de temperatura de alarme para todos setores (m = temperatura minima M = temperatura maxima)"},
  {"cala",             "                    -consulta as temperaturas de alarma dos setores"},
  {"aas s v",          "                    -ativa atuador do setor s (0 = todos setores) com variação ±v"},
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
    int argc = 0;
    int i = 0;

    //do nothing if user just presses space
    if (strcmp(command, "\n") == 0)
    {
        return;
    }

    //remove \n from the string
    command[strlen(command)-1] = '\0';

    //get space for token
    token = (char *)malloc(sizeof(command)*sizeof(char));
    if (token == NULL){
      printf("Erro ao alocar memoria\n");
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

    //get the fist word (command) and put it in message and arg1
    token = strtok(command, " ");
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
    free(token);

    // after all this we have:
    // argc with the number of args for switch case 
    // message with a standard format of "command argument argument ... argument argc"
    // arg1 which has just the command for strcmp

    switch (argc) {
        case 1:
            if(strcmp(arg1, "tsm") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            else if(strcmp(arg1, "sair") == 0)
            {
                printf("Client closing...\n");
                exit(0);        
            }
            else if(strcmp(arg1, "cala") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            else if(strcmp(arg1, "sos") == 0 || strcmp(arg1, "help") == 0)
            {
                for(i = 0; i < (int)NCOMMANDS; i++)
                {
                    printf("%s %s\n", commands[i].cmd_name, commands[i].cmd_help);
                }
                break;
            }
            printf("Intuti: comando invalido (argc = %d)\n", argc);
            break;
        case 2:
            if(strcmp(arg1, "cps") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            else if(strcmp(arg1, "cts") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            printf("Intuti: comando invalido (argc = %d)\n", argc);
            break;
        case 3:
            if(strcmp(arg1, "mpps") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            else if(strcmp(arg1, "mppa") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            else if(strcmp(arg1, "mppamb") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            else if(strcmp(arg1, "dala") == 0)
            {
                if (sendto(clisoc.sd, message, strlen(message)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
                break;
            }
            printf("Intuti: comando invalido (argc = %d)\n", argc);
            break;
        case 4:
            printf("Intuti: comando invalido (argc = %d)\n", argc);
            break;
        default:
            printf("Intuti: comando invalido (argc = %d)\n", argc);
            break;
    }

    free(message);
    free(arg1);
}

int main()
{
    clientsocket clisoc;
    pthread_t  thread;
    char buf[50];

    create_client_socket(&clisoc);

    if (pthread_create(&thread, NULL, thread_func, (void *)&clisoc) != 0) {
       printf("Erro a criar thread\n");
    }

    printf("Welcome\n\n");

    while(1)
    {
        fgets(buf, 50, stdin);
        send_command(buf, clisoc);
    }
    
    close(clisoc.sd);
    unlink(clisoc.usrname);

    return 0;
}