#include "client_socket.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define WORDSIZE 100
#define NCOMMANDS (sizeof(commands)/sizeof(struct command_d))

void *thread_func(void *clisoc)
{
    char buf [WORDSIZE];
    clientsocket *soc;
    soc = (clientsocket *)clisoc;
    while(1)
    {
        if (recvfrom(soc->sd, buf, WORDSIZE, 0, NULL, NULL) < 0) {
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

int main()
{
    clientsocket clisoc;
    pthread_t  thread;
    int i;
    char buf[50];

    create_client_socket(&clisoc);

    if (pthread_create(&thread, NULL, thread_func, (void *)&clisoc) != 0) {
       printf("Erro a criar thread\n");
    }

    printf("Welcome\n\n");

    while(1)
    {
        fgets(buf, 50, stdin);
        if(strcmp(buf, "sair\n") == 0) break;
        else if((strcmp(buf, "sos\n") == 0) || (strcmp(buf, "help\n") == 0)) {
            printf("lista de commandos\n");
            for(i = 0; i < NCOMMANDS; i++)
            {
                printf("%s %s\n", commands[i].cmd_name, commands[i].cmd_help);
            }
        } else if (sendto(clisoc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&clisoc.sis, clisoc.sislen) < 0) perror("CLI: Erro no sendto");
    }
    
    close(clisoc.sd);
    unlink(clisoc.usrname);

    return 0;
}