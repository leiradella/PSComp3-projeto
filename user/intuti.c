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

int main()
{
    clientsocket clisoc;
    pthread_t  thread;
    char buf[MAXMESSAGESIZE];

    create_client_socket(&clisoc);

    if (pthread_create(&thread, NULL, thread_func, (void *)&clisoc) != 0) {
       printf("Erro a criar thread\n");
    }

    printf("Welcome\n\n");

    while(1)
    {
        fgets(buf, MAXMESSAGESIZE, stdin);
        send_command(buf, clisoc);
    }
    
    close(clisoc.sd);
    unlink(clisoc.usrname);

    return 0;
}