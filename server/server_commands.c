/***************************************************************************
| File:   server_commands.c
|
| Group:  4
|
| Autors: Miguel Fernades  103024
|         Gonçalo Antunes  103524
|         Lucas Leiradella 103566
|
| Data:  Mar 2024
***************************************************************************/
#include "server_socket.h"
#include "server_threads.h"
#include "server_commands.h"
#include <sys/socket.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define WORDSIZE 100
#define MAXWORDS 50

extern int variavel_controlo_registo;

void tsm(serversocket servsock) //server exit
{
    char buf[WORDSIZE];

    snprintf(buf, WORDSIZE, "Server Closing...");
    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
    return;
}

void cala(serversocket servsock) //see min and max temperatures for alarm
{
    char buf[WORDSIZE];

    snprintf(buf, WORDSIZE, "minimum temp = %d\nmaximum temp = %d", tmin, tmax);
    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
}

void cts(serversocket servsock, thinput **threadinput, char** args) //see temperature of sector
{
    char buf[WORDSIZE];

    if(strcmp(args[1], "1") == 0)
    {
        pthread_mutex_lock(&mutex);
        snprintf(buf, WORDSIZE, "setor 1: temperatura = %d", threadinput[0]->TEMP);
        pthread_mutex_unlock(&mutex);
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } else if(strcmp(args[1], "2") == 0)
    {
        pthread_mutex_lock(&mutex);
        snprintf(buf, WORDSIZE,"setor 2: temperatura = %d", threadinput[1]->TEMP);
        pthread_mutex_unlock(&mutex);
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } else if(strcmp(args[1], "3") == 0)
    {
        pthread_mutex_lock(&mutex);
        snprintf(buf, WORDSIZE,"setor 3: temperatura = %d", threadinput[2]->TEMP);
        pthread_mutex_unlock(&mutex);
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } else if(strcmp(args[1], "0") == 0)
    {
        int i;
        pthread_mutex_lock(&mutex);
        for (i=0;i<NS;i++){
            snprintf(buf, WORDSIZE,"setor %d: temperatura = %d", i+1,threadinput[i]->TEMP);
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        }
        pthread_mutex_unlock(&mutex);
        return;
    }
    sprintf(buf, "setor invalido");
    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
}

void cps(serversocket servsock, thinput **threadinput, char** args) //see parameters of sector
{
    char buf[WORDSIZE];
    if(strcmp(args[1], "1") == 0)
    {
        pthread_mutex_lock(&mutex);
        snprintf(buf, WORDSIZE, "setor 1:\nestado = %c\nperiodo sensores = %d\nperiodo actuadores = %d\nperiodo ambiente = %d",threadinput[0]->tmanip,threadinput[0]->psen, threadinput[0]->pact, threadinput[0]->pamb);
        pthread_mutex_unlock(&mutex);
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } else if(strcmp(args[1], "2") == 0)
    {
        pthread_mutex_lock(&mutex);
        snprintf(buf, WORDSIZE, "setor 2:\nestado = %c\nperiodo sensores = %d\nperiodo actuadores = %d\nperiodo ambiente = %d",threadinput[1]->tmanip,threadinput[1]->psen, threadinput[1]->pact, threadinput[1]->pamb);
        pthread_mutex_unlock(&mutex);
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } else if(strcmp(args[1], "3") == 0)
    {
        pthread_mutex_lock(&mutex);
        snprintf(buf, WORDSIZE, "setor 3:\nestado = %c\nperiodo sensores = %d\nperiodo actuadores = %d\nperiodo ambiente = %d",threadinput[2]->tmanip,threadinput[2]->psen, threadinput[2]->pact, threadinput[2]->pamb);
        pthread_mutex_unlock(&mutex);
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } if(strcmp(args[1], "0") == 0)
    {
        int i;
        pthread_mutex_lock(&mutex);
        for (i=0;i<NS;i++){
            snprintf(buf, WORDSIZE, "setor %d: estado = %c\n periodo sensores = %d\n periodo actuadores = %d\n periodo ambiente = %d", i+1,threadinput[i]->tmanip,threadinput[i]->psen, threadinput[i]->pact, threadinput[i]->pamb);
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        }
        pthread_mutex_unlock(&mutex);
        return;
    }
    sprintf(buf, "setor invalido");
    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
}

void dala(serversocket servsock, char** args) //change min and max temp for alarm
{
    char buf[WORDSIZE];
    if (args[1] == NULL || args[2] == NULL)
    {
        snprintf(buf, WORDSIZE, "too few arguments");
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    }
    char *end = NULL;
    int num1 = (int)(strtol(args[1], &end, 10));
    if (*end != '\0') {
        snprintf(buf, WORDSIZE, "1st argument not a number");
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    }
    int num2 = (int)strtol(args[2], &end, 10);
    if (*end != '\0') {
        snprintf(buf, WORDSIZE, "2nd argument not a number");
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    }
    if (num1 >= num2)
    {
        snprintf(buf, WORDSIZE, "maximum temperature smaller than minimum temperature: tmin = %d tmax = %d", num1, num2);
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    }
    pthread_mutex_lock(&mutex);
    tmin = num1;
    tmax = num2;
    pthread_mutex_unlock(&mutex);
}

void cer (serversocket servsock)
{
    char buf[WORDSIZE];

    if (variavel_controlo_registo == 1)
    {
    snprintf(buf, WORDSIZE, "Envio para o registo esta ativo\n");
    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
    }
    else {
        snprintf(buf, WORDSIZE, "Envio para o registo esta desativado\n");
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
    }  
}

void aer(serversocket servsock)
{
    char buf[WORDSIZE];
    
    if (variavel_controlo_registo == 1)
    {
        snprintf(buf, WORDSIZE, "Envio para o registo ja esta ativo\n");
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
    } else {
        variavel_controlo_registo = 1;
        snprintf(buf, WORDSIZE, "Envio para o registo ativado\n");
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
    }
}

void der(serversocket servsock)
{
    char buf[WORDSIZE];
    
    if (variavel_controlo_registo == 0)
    {
        snprintf(buf, WORDSIZE, "Envio para o registo ja esta desativado\n");
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
    } else {
        variavel_controlo_registo = 0;
        snprintf(buf, WORDSIZE, "Envio para o registo desativado\n");
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
    }
}

void mpps(serversocket servsock, thinput **threadinput, char** args) //set sensor period time
{
    char buf[WORDSIZE];
    int arg1, arg2;

    // put 1st argument into arg1 and 2nd argument into arg2
    arg1 = (int)(strtol(args[1], NULL, 10));
    arg2 = (int)(strtol(args[2], NULL, 10));

    if (arg2 <= 0)
    {
        snprintf(buf, WORDSIZE, "period must be 1 or higher");
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return; 
    }

    //arg1 determines which sectors are affected by the command
    switch (arg1) {
        // all sectors
        case 0:
            for (int i = 0; i < arg2; i++)
            {
                threadinput[i]->psen = arg2;
            }
            break;
        case 1:
            threadinput[0]->psen = arg2;
            break;
        case 2:
            threadinput[1]->psen = arg2;
            break;
        case 3:
            threadinput[2]->psen = arg2;
            break;
        default:
            snprintf(buf, WORDSIZE, "Sismon: Setor invalido");
            break;
    }
}

void mppa(serversocket servsock, thinput **threadinput, char** args) //set actuator period time
{
    char buf[WORDSIZE];
    int arg1, arg2;

    // put 1st argument into arg1 and 2nd argument into arg2
    arg1 = (int)(strtol(args[1], NULL, 10));
    arg2 = (int)(strtol(args[2], NULL, 10));

    if (arg2 <= 0)
    {
        snprintf(buf, WORDSIZE, "Sismon: period must be 1 or higher");
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return; 
    }

    //arg1 determines which sectors are affected by the command
    switch (arg1) {
        // all sectors
        case 0:
            for (int i = 0; i < arg2; i++)
            {
                threadinput[i]->pact = arg2;
            }
            break;
        case 1:
            threadinput[0]->pact = arg2;
            break;
        case 2:
            threadinput[1]->pact = arg2;
            break;
        case 3:
            threadinput[2]->pact = arg2;
            break;
        default:
            snprintf(buf, WORDSIZE, "Sismon: Setor invalido");
            break;
    }
}

void mppamb(serversocket servsock, thinput **threadinput, char** args) //set actuator period time
{
    char buf[WORDSIZE];
    int arg1, arg2;

    // put 1st argument into arg1 and 2nd argument into arg2
    arg1 = (int)(strtol(args[1], NULL, 10));
    arg2 = (int)(strtol(args[2], NULL, 10));

    if (arg2 <= 0)
    {
        snprintf(buf, WORDSIZE, "period must be 1 or higher");
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return; 
    }

    //arg1 determines which sectors are affected by the command
    switch (arg1) {
        // all sectors
        case 0:
            for (int i = 0; i < arg2; i++)
            {
                threadinput[i]->pamb = arg2;
            }
            break;
        case 1:
            threadinput[0]->pamb = arg2;
            break;
        case 2:
            threadinput[1]->pamb = arg2;
            break;
        case 3:
            threadinput[2]->pamb = arg2;
            break;
        default:
            snprintf(buf, WORDSIZE, "Sismon: Setor invalido");
            break;
    }
}

void aas(serversocket servsock, thinput **threadinput, char** args)
{
    char buf[WORDSIZE];
    int arg1, arg2;
    char arg_sign;

    // put 1st argument into arg1 and 2nd argument into arg2
    arg1 = (int)(strtol(args[1], NULL, 10));
    arg2 = (int)(strtol(args[2], NULL, 10));

    if(arg2 < 0) {
        arg_sign = '-';
    } else if (arg2 == 0) {
        arg_sign = '0';
    } else if (arg2 > 0) {
        arg_sign = '+';
    }

    //arg1 determines which sectors are affected by the command
    switch (arg1) {
        // all sectors
        case 0:
            for (int i = 0; i < arg2; i++)
            {
                threadinput[i]->tmanip = arg_sign;
            }
            break;
        case 1:
            threadinput[0]->tmanip = arg_sign;
            break;
        case 2:
            threadinput[1]->tmanip = arg_sign;
            break;
        case 3:
            threadinput[2]->tmanip = arg_sign;
            break;
        default:
            snprintf(buf, WORDSIZE, "Sismon: Setor invalido");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            break;
    }
}

void handle_commands(char *command, serversocket servsock, thinput **threadinput)
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
    if (id == 0)
    {
        handler2(args, argc, threadinput, servsock);
    } else {
        handler1(args, argc, threadinput, servsock);
    }
    
    //free mallocs
    for(int i = 0; i < argc; i++)
    {
        free(args[i]);
    }
    free(args);
}

void handler1(char **args, int argc, thinput **threadinput, serversocket servsock)
{
    int id, i;
    char buf[WORDSIZE];

    //get the command id
    id = (int)strtol(args[0], NULL, 10);

    switch (argc) {
        case 1:
            switch (id) {
                case TSM:
                tsm(servsock);
                close(servsock.sd);
                unlink(servsock.sisname);
                for (i = 0; i < argc; i++)
                {
                    free(args[i]);
                }
                free(args);
                exit(0);
                case CALA:
                    cala(servsock);
                    break;
                case CER:
                    cer(servsock);
                    break;
                case AER:
                    aer(servsock);
                    break;
                case DER:
                    der(servsock);
                    break;
                default:
                    //if every compare fails, then the command doesnt exist
                    snprintf(buf, WORDSIZE, "Sismon: comando invalido(argc = %d)", argc);
                    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            }   
            break;
        case 2:
            switch (id) {
                case CPS:
                    cps(servsock, threadinput, args);
                    break;
                case CTS:
                    cts(servsock, threadinput, args);
                    break;
                default:
                    //if every compare fails, then the command doesnt exist
                    snprintf(buf, WORDSIZE, "Sismon: comando invalido(argc = %d)", argc);
                    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            }
            break;
        case 3:
            switch (id) {
                case AAS:
                    aas(servsock, threadinput, args);
                    break;
                case DALA:
                    dala(servsock, args);
                    break;
                case MPPS: 
                    mpps(servsock, threadinput, args);
                    break;
                case MPPA:
                    mppa(servsock, threadinput, args);
                    break;
                case MPPAMB:
                    mppamb(servsock, threadinput, args);
                    break;
                default:
                    //if every compare fails, then the command doesnt exist
                    snprintf(buf, WORDSIZE, "Sismon: comando invalido(argc = %d)", argc);
                    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            }  
            break;
        default:
            snprintf(buf, WORDSIZE, "Sismon: Comando invalido (argc = %d)", argc);
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            break;
    }
}

void handler2(char **args, int argc, thinput **threadinput, serversocket servsock)
{
    int i;
    char buf[WORDSIZE];

    switch(argc) {
    case 1:
        if (strcmp(args[0], "tsm") == 0)
        {
            tsm(servsock);
            close(servsock.sd);
            unlink(servsock.sisname);
            for (i = 0; i < argc; i++)
            {
                free(args[i]);
            }
            free(args);
            exit(0);
        }
        else if (strcmp(args[0], "cala") == 0) {
            cala(servsock);
            break;
        }
        else if (strcmp(args[0], "cer") == 0) {
            cer(servsock);
            break;
        }
        else if (strcmp(args[0], "aer") == 0) {
            aer(servsock);
            break;
        }
        else if (strcmp(args[0], "der") == 0) {
            der(servsock);
            break;
        }
        //if every compare fails, then the command doesnt exist
        snprintf(buf, WORDSIZE, "Sismon: comando invalido (argc = %d)", argc);
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        break;
    case 2:
        if (strcmp(args[0], "cts") == 0)
        {
            cts(servsock, threadinput, args);
            break;
        }
        if (strcmp(args[0], "cps") == 0) { 
            cps(servsock, threadinput, args);
            break;
        }
        //if every compare fails, then the command doesnt exist
        snprintf(buf, WORDSIZE, "Sismon: comando invalido (argc = %d)", argc);
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        break;
    case 3:
        if (strcmp(args[0], "dala") == 0)
        {
            dala(servsock, args);
            break;
        }
        if (strcmp(args[0], "mpps") == 0) {
            mpps(servsock, threadinput, args);
            break;
        }
        if (strcmp(args[0], "mppa") == 0) {
            mppa(servsock, threadinput, args);
            break;
        }
        if (strcmp(args[0], "mppamb") == 0) {
            mppamb(servsock, threadinput, args);
            break;
        }
        //if every compare fails, then the command doesnt exist
        snprintf(buf, WORDSIZE, "Sismon: comando invalido(argc = %d)", argc);
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        break;
    default:
        snprintf(buf, WORDSIZE, "Sismon: Comando invalido (argc = %d)", argc);
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        break;
    }
}

//checks if reghist is running. if not, then starts reghist
void check_reghist()
{
    char command[WORDSIZE];
    pid_t pid;
    
    snprintf(command, WORDSIZE, "ps -C reghist > /dev/null");

    if (system(command) != 0)
    {
        snprintf(command, WORDSIZE, "../reghist/reghist");
        
        pid = fork();
        if (pid == 0)
        {
            system(command);
            exit(0);
        }
    }
}
