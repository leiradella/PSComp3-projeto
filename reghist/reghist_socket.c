#include "reghist_socket.h"
#include <unistd.h>
#include <stdio.h>

#define INTUTI "/tmp/INTUTI"
#define REGHIST "/tmp/reghist"

void create_reghist_socket(struct ReghistSocket *soc)
{
    strcpy(soc->regname, REGHIST);
    unlink(soc->regname);

    if ((soc->sd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0 ) {
        perror("Erro a criar socket"); exit(-1);
    }
        
    soc->reg.sun_family = AF_UNIX;
    memset(soc->reg.sun_path, 0, sizeof(soc->reg.sun_path));
    strcpy(soc->reg.sun_path, soc->regname);
    soc->reglen = (socklen_t)(sizeof(soc->reg.sun_family) + strlen(soc->reg.sun_path));

    if (bind(soc->sd, (struct sockaddr *)&soc->reg, soc->reglen) < 0 ) {
        perror("Erro no bind"); exit(-1);
    }

    soc->fromlen = sizeof(soc->from);
}
