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
  int f= 0;

  
  //criacao do socket
  serversocket servsock;
  
  sock_create(&servsock);

  // criacao das threads
  threadinput *threadinput[NS];
  pthread_t threads[NT*NS];

for (i = 0; i < 3; i++)
{
  threadinput[i] = (struct Threadinputs *)malloc(sizeof(struct Threadinputs));
    if (threadinput[i] == NULL){
      printf("Erro ao alocar memoria\n");
      return EXIT_FAILURE;
    }
}
  threadinput[0]->TEMP = TINI;
  threadinput[0]->tmanip = 0;
  threadinput[1]->TEMP = TINI;
  threadinput[1]->tmanip = 0;
  threadinput[2]->TEMP = TINI;
  threadinput[2]->tmanip = 0;
  threadinput[0]->id = 1;
  threadinput[1]->id = 2;
  threadinput[2]->id = 3;
  
while (f < NS*NT)
{

for (i = 0; i < NS; i++)
{
  if (pthread_create(&threads[f], NULL, thread_sen, (void*)threadinput[i]) != 0) 
  {
    printf("Erro ao criar thread sensor"); 
  }
  f++;
  if (pthread_create(&threads[f], NULL, thread_act, (void*)threadinput[i])  != 0) 
  {
    printf("Erro a criar thread atuador"); 
  }
  f++;
  if (pthread_create(&threads[f], NULL, thread_amb, (void*)threadinput[i])  != 0) 
  {
    printf("Erro a criar thread ambiente"); 
  }
  f++;
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
