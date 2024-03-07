#include "client_socket.h"
#include <unistd.h>
#include <stdio.h>

int main()
{
    clientsocket clisoc;
    char buf[50];
    create_client_socket(&clisoc);

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