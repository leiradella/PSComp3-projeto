#include "server_socket.h"
#include "server_threads.h"
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
        //mutex_lock()
        snprintf(buf, WORDSIZE, "setor 1: temperatura = %d", threadinput[0]->TEMP);
        //mutex_unlock()
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } else if(strcmp(args[1], "2") == 0)
    {
        //mutex_lock()
        snprintf(buf, WORDSIZE,"setor 2: temperatura = %d", threadinput[1]->TEMP);
        //mutex_unlock()
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } else if(strcmp(args[1], "3") == 0)
    {
        //mutex_lock()
        snprintf(buf, WORDSIZE,"setor 3: temperatura = %d", threadinput[2]->TEMP);
        //mutex_unlock()
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } else if(strcmp(args[1], "0") == 0)
    {
        int i;
        //mutex_lock()
        for (i=0;i<NS;i++){
            snprintf(buf, WORDSIZE,"setor %d: temperatura = %d", i+1,threadinput[i]->TEMP);
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        }
        //mutex_unlock()
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
        //mutex_lock()
        snprintf(buf, WORDSIZE, "setor 1:\nestado = %c\nperiodo sensores = %d\nperiodo actuadores = %d\nperiodo ambiente = %d",threadinput[0]->tmanip,threadinput[0]->psen, threadinput[0]->pact, threadinput[0]->pamb);
        //mutex_unlock()
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } else if(strcmp(args[1], "2") == 0)
    {
        //mutex_lock()
        snprintf(buf, WORDSIZE, "setor 2:\nestado = %c\nperiodo sensores = %d\nperiodo actuadores = %d\nperiodo ambiente = %d",threadinput[1]->tmanip,threadinput[1]->psen, threadinput[1]->pact, threadinput[1]->pamb);
        //mutex_unlock()
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } else if(strcmp(args[1], "3") == 0)
    {
        //mutex_lock()
        snprintf(buf, WORDSIZE, "setor 3:\nestado = %c\nperiodo sensores = %d\nperiodo actuadores = %d\nperiodo ambiente = %d",threadinput[2]->tmanip,threadinput[2]->psen, threadinput[2]->pact, threadinput[2]->pamb);
        //mutex_unlock()
        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        return;
    } if(strcmp(args[1], "0") == 0)
    {
        int i;
        //mutex_lock()
        for (i=0;i<NS;i++){
            snprintf(buf, WORDSIZE, "setor %d: estado = %c\n periodo sensores = %d\n periodo actuadores = %d\n periodo ambiente = %d", i+1,threadinput[i]->tmanip,threadinput[i]->psen, threadinput[i]->pact, threadinput[i]->pamb);
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
        }
        //mutex_unlock()
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
    if (strcmp(args[1], "1") == 0)
    {
        char *end;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must be a positive value");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        threadinput[0]->psen = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    if (strcmp(args[1], "2") == 0)
    {
        char *end;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must be a positive value");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        threadinput[1]->psen = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    if (strcmp(args[1], "3") == 0)
    {
        char *end;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must be a positive value");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        threadinput[2]->psen = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    if (strcmp(args[1], "0") == 0)
    {
        char *end;
        int i;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must me at least 1");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        for(i = 0;i<3;i++) threadinput[i]->psen = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    snprintf(buf, WORDSIZE, "1st argument invalid");
    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
    return; 
}

void mppa(serversocket servsock, thinput **threadinput, char** args) //set actuator period time
{
    char buf[WORDSIZE];
    if (strcmp(args[1], "1") == 0)
    {
        char *end;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must be a positive value");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        threadinput[0]->pact = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    if (strcmp(args[1], "2") == 0)
    {
        char *end;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must be a positive value");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        threadinput[1]->pact = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    if (strcmp(args[1], "3") == 0)
    {
        char *end;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must be a positive value");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        threadinput[2]->pact = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    if (strcmp(args[1], "0") == 0)
    {
        char *end;
        int i;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must me at least 1");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        for(i=0;i<3;i++) threadinput[i]->pact = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    snprintf(buf, WORDSIZE, "1st argument invalid");
    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
    return; 
}

void mppamb(serversocket servsock, thinput **threadinput, char** args) //set actuator period time
{
    char buf[WORDSIZE];
    if (strcmp(args[1], "1") == 0)
    {
        char *end;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must be a positive value");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        threadinput[0]->pamb = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    if (strcmp(args[1], "2") == 0)
    {
        char *end;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must be a positive value");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        threadinput[1]->pamb = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    if (strcmp(args[1], "3") == 0)
    {
        char *end;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must be a positive value");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        threadinput[2]->pamb = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    if (strcmp(args[1], "0") == 0)
    {
        char *end;
        int i;
        int num = (int)(strtol(args[2], &end, 10));
        if (*end != '\0') {
            snprintf(buf, WORDSIZE, "2nd argument not a number");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        }
        if (num <= 0)
        {
            snprintf(buf, WORDSIZE, "period must me at least 1");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return; 
        }
        pthread_mutex_lock(&mutex);
        for(i=0;i<3;i++) threadinput[i]->pamb = num;
        pthread_mutex_unlock(&mutex);
        return;
    }
    snprintf(buf, WORDSIZE, "1st argument invalid");
    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
    return; 
}

void handle_commands(char *command, serversocket servsock, thinput **threadinput)
{
    int j = 0; 
    int argc = 0;
    char **args;
    char *token;
    char buf[WORDSIZE];

    // make space for token
    token = (char *)malloc(sizeof(command)*sizeof(char));
    if (token == NULL){
      printf("Erro ao alocar memoria\n");
      return;
    }

    // make space for args
    args = (char **)malloc(MAXWORDS*sizeof(char*));
    if (args == NULL){
      printf("Erro ao alocar memoria\n");
      return;
    }

    //get first word (command)
    token = strtok(command, " ");

    //put all words of command into args
    while(token != NULL)
    {
        args[argc] = (char *)malloc(sizeof(token)*sizeof(char));
        if (args[argc] == NULL){
            printf("Erro ao alocar memoria\n");
            return;
        }
        strcpy(args[argc], token);
        token = strtok(NULL, " ");
        argc++;
    }

    switch(argc) {
        case 1:
            if (strcmp(args[0], "tsm") == 0)
            {
                tsm(servsock);
                
                close(servsock.sd);
                unlink(servsock.sisname);
                for (j = 0; j < argc; j++)
                {
                    free(args[j]);
                }
                free(args);
                free(token);
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
            snprintf(buf, WORDSIZE, "comando invalido");
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
            snprintf(buf, WORDSIZE, "comando invalido");
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
            snprintf(buf, WORDSIZE, "comando invalido");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            break;
        default:
            snprintf(buf, WORDSIZE, "Sismon: Comando invalido (argc = %d)", argc);
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            break;
    }
    
    for (j = 0; j < argc; j++)
    {
        free(args[j]);
    }
    free(args);
    free(token);
}
