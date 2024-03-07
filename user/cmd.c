#include "client_socket.h"
#include <stdio.h>
=======
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>



extern void monitor(void);

int main(void)
{

  create_client_socket();
  monitor();

  return 0;
}
