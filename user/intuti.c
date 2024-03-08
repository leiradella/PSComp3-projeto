#include "client_socket.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define WORDSIZE 100

void *thread_func(void *clisoc)
{
    char buf [WORDSIZE];
    clientsocket *soc;
    soc = (clientsocket *)clisoc;
    while(1)
    {
        if (recvfrom(soc->sd, buf, sizeof(WORDSIZE), 0, NULL, NULL) < 0) {
            perror("Erro no recvfrom");
        }
        printf("%s\n", buf);
    }
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

    while(1)
    {
        fgets(buf, 50, stdin);
        if(strcmp(buf, "client_exit\n") == 0) break;
        if (sendto(clisoc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&clisoc.to, clisoc.tolen) < 0) perror("CLI: Erro no sendto");
    }
    
    close(clisoc.sd);
    unlink(clisoc.cliname);

    return 0;
}