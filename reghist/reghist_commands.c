#include "reghist_commands.h"
#include "reghist.h"
#include <errno.h>
#include <stdio.h>

#define MAXWORDS 50
#define WORDSIZE 100

void trh(regsocket soc)
{
    char buf[WORDSIZE];

    snprintf(buf, WORDSIZE, "Reghist closing...");
    if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
}

void lreg(char *pa, reg_t registo, char **args, int argc, regsocket soc)
{
    char buf[WORDSIZE];

    switch (argc) {
        case 3:
            break;
        case 4:
            break;
        default:
            snprintf(buf, WORDSIZE, "Reghist: lreg invalid number of arguments = %d (must be 2 or 3)", argc-1);
            if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
    }
}

void handle_commands(char *pa, reg_t registo, char *command, regsocket soc)
{
    char *token, **args;
    int id, argc = 0;

    // make space for args
    args = (char **)malloc(MAXWORDS*sizeof(char*));
    if (args == NULL){
      printf("Erro ao alocar memoria\n");
      return;
    }

    //get first word of string (the command)
    token = strtok(command, " ");
    
    //put all words of commands in args
    while (token != NULL)
    {
        //make space for current word
        args[argc] = (char *)malloc(WORDSIZE*sizeof(char));
        if (args == NULL){
            printf("Erro ao alocar memoria\n");
            return;
        }
        strcpy(args[argc], token);
        token = strtok(NULL, " ");
        argc++;
    }
    
    id = (int)strtol(args[0], NULL, 10);
    //if the first word inst a valid id for a reghist command, then it must be a string command
    if (id != LREG && id != TRH)
    {
        handler2(pa, registo, args, argc, soc);
    } else {
        handler1(pa, registo, args, id, argc, soc);
    }
    
    //free mallocs
    for(int i = 0; i < argc; i++)
    {
        free(args[i]);
    }
    free(args);
}

void handler1(char *pa, reg_t registo, char **args, int id, int argc,regsocket soc)
{
    char buf[WORDSIZE];

    switch (id){
        case TRH:
            trh(soc);
            break;
        case LREG:
            lreg(pa, registo, args, argc, soc);
            break;
        default:
            snprintf(buf, WORDSIZE, "Reghist: Invalid command id (mode 1)");
            if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
    }
        
}

void handler2(char *pa, reg_t registo, char **args, int argc, regsocket soc)
{
    char buf[WORDSIZE];

    switch (argc) {
        case 1:
            if (strcmp(args[0], "trh") == 0)
            {
                trh(soc);
            }
            break;
        case 3:
            if (strcmp(args[0], "lreg") == 0)
            {
                lreg(pa, registo, args, argc, soc);
            }
        case 4:
            if (strcmp(args[0], "lreg") == 0)
            {
                lreg(pa, registo, args, argc, soc);
            }
            break;
        default:
            snprintf(buf, WORDSIZE, "Reghist: Invalid command (mode 2)");
            if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
    }
}
