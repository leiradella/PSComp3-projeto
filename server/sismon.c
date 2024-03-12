#include "server_commands.h"
#include "server_socket.h"
#include "server_threads.h"
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int main ()
{   
  char command[100];
  int i;

  
  //criacao do socket
  serversocket servsock;
  
  sock_create(&servsock);

  // criacao das threads
  thinput *threadinput[NS];
  pthread_t threads[NS][NT];

for (i = 0; i < NS; i++)
{
  threadinput[i] = (struct Threadinputs *)malloc(sizeof(struct Threadinputs));
    if (threadinput[i] == NULL){
      printf("Erro ao alocar memoria\n");
      return EXIT_FAILURE;
    }
}
for(i=0; i < NS; i++)
{
  threadinput[i]->tmanip = '+';
  threadinput[i]->TEMP = TINI;
  threadinput[i]->psen = PSEN;
  threadinput[i]->pact = PACT;
  threadinput[i]->pamb = PAMB;
  threadinput[i]->id = i+1;
}
  
for (i = 0; i < NS; i++)
{
  if (pthread_create(&threads[i][0], NULL, thread_sen, (void*)threadinput[i]) != 0) 
  {
    printf("Erro ao criar thread sensor"); 
  }
  if (pthread_create(&threads[i][1], NULL, thread_act, (void*)threadinput[i])  != 0) 
  {
    printf("Erro a criar thread atuador"); 
  }
  if (pthread_create(&threads[i][2], NULL, thread_amb, (void*)threadinput[i])  != 0) 
  {
    printf("Erro a criar thread ambiente"); 
  } 
 }
  while (1)
  { 
    if (recvfrom(servsock.sd, command, sizeof(command), 0, (struct sockaddr *)&servsock.from, &servsock.fromlen) < 0) {
    perror("Erro no recvfrom");
    }
    handle_commands(command, servsock, threadinput);
  }

  close(servsock.sd);
  unlink(servsock.servname);

  return 0;
}
