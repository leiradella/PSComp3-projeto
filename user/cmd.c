#include "client_socket.h"
#include <stdio.h>
<<<<<<< Updated upstream
=======
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>


>>>>>>> Stashed changes

extern void monitor(void);

int main(void)
{
<<<<<<< Updated upstream
=======
  create_client_socket();
>>>>>>> Stashed changes
  monitor();

  return 0;
}
