#include "naosei.h"
#include "server_socket.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int main ()
{   
  char buf[100];
  
  //criacao do socket
  serversocket servsock;
  
  sock_create(&servsock);

  // criacao das threads
  threadinput *threadinput1;
  pthread_t  threads[NT];

  threadinput1 = (struct Threadinputs *)malloc(sizeof(struct Threadinputs));
    if (threadinput1 == NULL){
      printf("Erro ao alocar memoria\n");
      return EXIT_FAILURE;
    }

  threadinput1->TEMP = TINI;
  threadinput1->tmanip = 0;

  if (pthread_create(&threads[0], NULL, thread_sen, (void*)threadinput1) != 0) 
  {
    printf("Erro ao criar thread sensor"); 
  }
  
  if (pthread_create(&threads[1], NULL, thread_act, (void*)threadinput1) != 0) 
  {
    printf("Erro a criar thread atuador"); 
  }
 
  if (pthread_create(&threads[2], NULL, thread_amb, (void*)threadinput1) != 0) 
  {
    printf("Erro a criar thread ambiente"); 
  }
      
  while (1)
  { 
    if (recvfrom(servsock.sd, buf, sizeof(buf), 0, (struct sockaddr *)&servsock.from, &servsock.fromlen) < 0) {
    perror("Erro no recvfrom");
    }
    if (strcmp(buf, "server_exit") == 0)
    {
      break;
    }
  }

  close(servsock.sd);
  unlink(servsock.servname);

  return 0;
}
