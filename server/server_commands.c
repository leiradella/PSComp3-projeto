#include "server_socket.h"
#include "server_threads.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define WORDSIZE 100
#define MAXWORDS 50

void handle_commands(char *command, serversocket servsock, thinput **threadinput)
{
    int commandchar = 0; //current character from command string
    int argchar = 0; //current character from arg string
    int argword = 0; //current string from string vector
    int word_num = 0; //how many words in command
    char **args;
    char *buf;

    args = malloc(MAXWORDS);
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
        if (command[commandchar] == '\0' || command[commandchar] == '\n')
        {
            printf("No words in this string\n");
            return;
        }

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
            printf("no words\n"); 
            return;
        case 1:
            if(strcmp(args[0], "tsm") == 0)
            {
                printf("Server closing...\n");
                close(servsock.sd);
                unlink(servsock.servname);
                exit(0);
            }
            return;
        case 2:
            if(strcmp(args[0], "cts") == 0)
            {
                if(strcmp(args[1], "0") == 0)
                {
                    buf = (char *)malloc(WORDSIZE);
                    if (buf == NULL){
                        printf("Erro ao alocar memoria\n");
                        return;
                    }
                    //mutex_lock()
                    sprintf(buf, "setor 0: temperatura = %d", threadinput[0]->TEMP);
                    //mutex_unlock()
                    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
                    return;
                } else if(strcmp(args[1], "1") == 0)
                {
                    buf = (char *)malloc(WORDSIZE);
                    if (buf == NULL){
                        printf("Erro ao alocar memoria\n");
                        return;
                    }
                    //mutex_lock()
                    sprintf(buf, "setor 1: temperatura = %d", threadinput[1]->TEMP);
                    //mutex_unlock()
                    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
                    return;
                } else if(strcmp(args[1], "2") == 0)
                {
                    buf = (char *)malloc(WORDSIZE);
                    if (buf == NULL){
                        printf("Erro ao alocar memoria\n");
                        return;
                    }
                    //mutex_lock()
                    sprintf(buf, "setor 2: temperatura = %d", threadinput[2]->TEMP);
                    //mutex_unlock()
                    if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
                    return;
                } if(strcmp(args[1], "todos") == 0)
                {
                    buf = (char *)malloc(WORDSIZE);
                    if (buf == NULL){
                        printf("Erro ao alocar memoria\n");
                        return;
                    }
                    int i;
                    //mutex_lock()
                    for (i=0;i<NS;i++){
                        sprintf(buf, "setor %d: temperatura = %d", i,threadinput[i]->TEMP);
                        if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
                    }
                    //mutex_unlock()
                    return;
                }
                buf = (char *)malloc(WORDSIZE);
                if (buf == NULL){
                    printf("Erro ao alocar memoria\n");
                    return;
                }
                sprintf(buf, "setor invalido");
                if (sendto(servsock.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&servsock.from, servsock.fromlen) < 0) perror("SERV: Erro no sendto");
            }
            return;
        default:
            printf("invalid command\n"); 
            return;
    }
}