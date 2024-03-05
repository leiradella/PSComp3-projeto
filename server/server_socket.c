#include "server_socket.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

//this creates a socket for the program
void sock_create(struct ServerSocket *soc)
{
    sprintf(soc->servname,"/tmp/SERV");

    if ((soc->sd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0 ) 
    {
        perror("Erro a criar socket"); exit(-1);
    }
    soc->my_addr.sun_family = AF_UNIX;
    memset(soc->my_addr.sun_path, 0, sizeof(soc->my_addr.sun_path));
    strcpy(soc->my_addr.sun_path, soc->servname);
    soc->addrlen = (size_t)sizeof(soc->my_addr.sun_family) + strlen(soc->my_addr.sun_path);
    
    if (bind(soc->sd, (struct sockaddr *)&soc->my_addr, soc->addrlen) < 0 ) 
    {
        //when bind fails its generally because address is already in use
        //change servname name in case current name is  already taken
        change_servname(soc->servname);
    }

    soc->fromlen = sizeof(soc->from);


}

void change_servname(char* servname)
{
    int len = (int)strlen(servname);
    int i = len - 1;
    
    // Iterate backwards from the end of the string
    while (i >= 0 && isdigit(servname[i])) {
        // If the character is a digit, increment it
        if (servname[i] != '9') {
            servname[i]++;
            return; // Exit function after incrementing the digit
        } else {
            // If the digit is '9', set it to '0' and continue to the previous digit
            servname[i] = '0';
            i--;
        }
    }
}
