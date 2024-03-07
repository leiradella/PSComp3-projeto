#include "client_socket.h"
#include <unistd.h>

int main()
{
    clientsocket clisoc;
    create_client_socket(&clisoc);
    sendto(clisoc.sd, "server_exit",strlen("server_exit"), 0, (struct sockaddr *)&clisoc.to, clisoc.tolen);
    
    close(clisoc.sd);
    unlink(clisoc.cliname);

    return 0;
}