#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#define SERVNAME "/tmp/SERV"
#define CLINAME "/tmp/CLI"

extern void monitor(void);

int main(void)
{
  int sd;
  struct sockaddr_un my_addr;
  socklen_t addrlen;
  struct sockaddr_un to;
  socklen_t tolen;
  char buf[100];

  if ((sd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0 ) {
    perror("Erro a criar socket"); exit(-1);
  }
      
  my_addr.sun_family = AF_UNIX;
  memset(my_addr.sun_path, 0, sizeof(my_addr.sun_path));
  strcpy(my_addr.sun_path, CLINAME);
  addrlen = sizeof(my_addr.sun_family) + strlen(my_addr.sun_path);

  if (bind(sd, (struct sockaddr *)&my_addr, addrlen) < 0 ) {
    perror("Erro no bind"); exit(-1);
  }

  to.sun_family = AF_UNIX;
  memset(to.sun_path, 0, sizeof(to.sun_path));
  strcpy(to.sun_path, SERVNAME);
  tolen = sizeof(my_addr.sun_family) + strlen(to.sun_path);
  monitor();

  return 0;
}
