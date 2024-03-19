#include "server_commands.h"
#include "server_socket.h"
#include "server_threads.h"
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include <mqueue.h>

#define REGQ "/REGQ"
#define MAX_MSG_SIZE sizeof(registo_queue)

mqd_t mq;
int variavel_controlo_registo;

int main ()
{   
  char command[100];
  int i;
  struct mq_attr attr;

  serversocket servsock;
  thinput *threadinput[NS];
   
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MAX_MSG_SIZE;
  attr.mq_curmsgs = 0;
  
    //criacao do socket
    sock_create(&servsock);

    // criacao das threads
    pthread_t threads[NS][NT];

    // Criando a fila de mensagens
    mq = mq_open(REGQ, O_CREAT | O_WRONLY, 0666, &attr);
      if (mq == -1) {
        perror("Erro na criação da queue \n");
        exit(1);
      }
    variavel_controlo_registo = 1;

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
    handle_commands(command, servsock, threadinput, mq);
  }

  close(servsock.sd);
  unlink(servsock.servname);

  return 0;
}
