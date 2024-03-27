/***************************************************************************
| File:   intuti.c
|
| Group:  4
|
| Autors: Miguel Fernades  103024
|         Gonçalo Antunes  103524
|         Lucas Leiradella 103566
|
| Data:  Mar 2024
***************************************************************************/
#include "client_socket.h"
#include "client_commands.h"
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define MAXMESSAGESIZE 200

//this function receives messages from reghist and sismon without locking the user from writing commands
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

//in this function we initialize the socket, start the thread that receives messages, and receive inputs from the keyboard to send to the correct entity.
int main()
{
    clientsocket clisoc;
    pthread_t  thread;
    char buf[MAXMESSAGESIZE];

    //create socket
    create_client_socket(&clisoc);

    //create thread that receives
    if (pthread_create(&thread, NULL, thread_func, (void *)&clisoc) != 0) {
       printf("Erro a criar thread\n");
    }

    printf("Welcome\n\n");

    //reveceive user commands and send them to sismon or reghist
    while(1)
    {
        fgets(buf, MAXMESSAGESIZE, stdin);
        send_command(buf, clisoc);
    }
    
    close(clisoc.sd);
    unlink(clisoc.usrname);

    return 0;
}