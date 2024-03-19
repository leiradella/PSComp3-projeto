#include "server_socket.h"
#include "server_threads.h"
#include <sys/socket.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>

#define WORDSIZE 100
#define MAXWORDS 50


void tsm(serversocket servsock) //server exit
{
    char buf[WORDSIZE];

    snprintf(buf, WORDSIZE, "Server Closing...");
    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
    close(servsock.sd);
    unlink(servsock.servname);
    exit(0);
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

void handle_commands(char *command, serversocket servsock, thinput **threadinput, mqd_t mq)
{
    int commandchar = 0; //current character from command string
    int argchar = 0; //current character from arg string
    int argword = 0; //current string from string vector
    int word_num = 0; //how many words in command
    char **args;
    char buf[WORDSIZE];

    args = malloc(MAXWORDS*sizeof(char*));
    if (args == NULL){ 
        printf("Erro ao alocar memoria\n");
        return;
    }

    //go through the whole string
    while (command[commandchar] != '\0' && command[commandchar] != '\n')
    {
        //jump spaces
        while(command[commandchar] == ' ')
        {
            commandchar++;
        }

        //check if string isnt just a bunch of spaces
        if ((command[commandchar] == '\0' || command[commandchar] == '\n') && word_num == 0)
        {
            snprintf(buf, WORDSIZE,"string is just a bunch of spaces");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        } else if (command[commandchar] == '\0' || command[commandchar] == '\n') break;

        //make space for word in string vector
        args[argword] = (char *)malloc(WORDSIZE);
        if (args[argword] == NULL){
            printf("Erro ao alocar memoria\n");
            return;
        }

        argchar = 0;
        //word has begun
        while(command[commandchar] != '\0' && command[commandchar] != ' ' && command[commandchar] != '\n')
        {
            //fills one string in string vector with word from command
            args[argword][argchar] = command[commandchar];
            argchar++;
            commandchar++;
        }
        args[argword][argchar] = '\0'; //worrds ends in EOF
        argword++;
        word_num++; //to keep count of how many words
    }

    switch(word_num) {
        case 0:
            return;
        case 1:
            if (strcmp(args[0], "tsm") == 0)
            {
                tsm(servsock);
                return;
            }
            else if (strcmp(args[0], "cala") == 0) {
                cala(servsock);
                return;
            }
            //if every compare fails, then the command doesnt exist
            snprintf(buf, WORDSIZE, "comando invalido");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        case 2:
            if (strcmp(args[0], "cts") == 0)
            {
                cts(servsock, threadinput, args);
                return;
            }
            if (strcmp(args[0], "cps") == 0) { 
                cps(servsock, threadinput, args);
                return;
            }
            //if every compare fails, then the command doesnt exist
            snprintf(buf, WORDSIZE, "comando invalido");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        case 3:
            if (strcmp(args[0], "dala") == 0)
            {
                dala(servsock, args);
                return;
            }
            if (strcmp(args[0], "mpps") == 0) {
                mpps(servsock, threadinput, args);
                return;
            }
            if (strcmp(args[0], "mppa") == 0) {
                mppa(servsock, threadinput, args);
                return;
            }
            if (strcmp(args[0], "mppamb") == 0) {
                mppamb(servsock, threadinput, args);
                return;
            }
            //if every compare fails, then the command doesnt exist
            snprintf(buf, WORDSIZE, "comando invalido");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
        default:
            snprintf(buf, WORDSIZE, "FUCK YOU YOU FUCKING DICK");
            if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            return;
    }
}
