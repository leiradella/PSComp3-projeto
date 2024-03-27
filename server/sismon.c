/***************************************************************************
| File:   sismon.c
|
| Group:  4
|
| Autors: Miguel Fernades  103024
|         Gonçalo Antunes  103524
|         Lucas Leiradella 103566
|
| Data:  Mar 2024
***************************************************************************/
#include "server_commands.h"
#include "server_socket.h"
#include "server_threads.h"
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>

#define REGQ "/REGQ"
#define WORDSIZE 100
#define MAX_MSG_SIZE sizeof(registo_queue)
#define TIMEOUT_SEC 5

mqd_t mq;
int variavel_controlo_registo;
int sigterm_signal = 0;

// Signal handler function, this enables a flag that will terminate the main loop and close everything accordingly
void sighand(int signum) {
  printf("Received SIGTERM signal (%d). Server Exiting...\n", signum);
  sigterm_signal = 1;
}


//here we initialize the socket, create the sector threads, define the signal and open the queue for reghist, after that its just a loop of waiting for intuti command with recvfrom
int main ()
{   
  char command[WORDSIZE];
  int i;
  struct mq_attr attr;
  struct timeval timeout = {TIMEOUT_SEC, 0};
  ssize_t msg_status;
  
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MAX_MSG_SIZE;
  attr.mq_curmsgs = 0;

  // criacao do signal
  signal(SIGTERM, sighand);

  //criacao do socket
  serversocket servsock;
  sock_create(&servsock);

  // Set timeout option for recvfrom
  if (setsockopt(servsock.sd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }

  // Criando a fila de mensagens
  mq = mq_open(REGQ, O_CREAT | O_WRONLY, 0666, &attr);
  if (mq == -1) {
    perror("Erro na criação da queue \n");
    exit(1);
  }
  
  check_reghist();
  variavel_controlo_registo =1;

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

//this starts every threadinput function with the initial values while giving them their sector ids, in our case NS = 3, so we have 3 threadinputs with ids 1, 2 and 3
for(i=0; i < NS; i++)
{
  threadinput[i]->tmanip = '+';
  threadinput[i]->TEMP = TINI;
  threadinput[i]->psen = PSEN;
  threadinput[i]->pact = PACT;
  threadinput[i]->pamb = PAMB;
  threadinput[i]->id = i+1;
}

//here we create the threads and send the correct threadinputs to them.
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
  while (!sigterm_signal)
  { 
    // msg_status stores recvfrom value to check if it was succesful or not
    msg_status = recvfrom(servsock.sd, command, sizeof(command), 0, (struct sockaddr *)&servsock.from, &servsock.fromlen);
    
    // if msg_status < 0, then an error ocurred, now we check if the error is related to the timeout, we dont want sismon to keep printing timeout so we just commented it
    if (msg_status < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) 
    {
      //printf("timeout\n");
    } 
    else if (msg_status < 0) 
    {
      perror("Erro no recvfrom:");
    } 
    else 
    {  
      handle_commands(command, servsock, threadinput);
    }
  }

  for (i = 0; i < NS; i++)
  {
    free(threadinput[i]);
  }
  mq_close(mq);
  close(servsock.sd);
  unlink(servsock.sisname);

  return 0;
}
