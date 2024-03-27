/***************************************************************************
| File:   reghist.c
|
| Group:  4
|
| Autors: Miguel Fernades  103024
|         Gonçalo Antunes  103524
|         Lucas Leiradella 103566
|
| Data:  Mar 2024
***************************************************************************/
#include "reghist.h"
#include "reghist_socket.h"
#include "reghist_commands.h"
#include <sys/time.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define TIMEOUT_SEC 5

#define SIZE sizeof(timespec_t);

int sigterm_signal = 0;
reg_t *pa;
reg_t registo;

// Signal handler function
void sighand(int signum) {
  printf("Received SIGTERM signal (%d). Reghist Exiting...\n", signum);
  sigterm_signal = 1;
}

//writes in the current entry in mmap, also retrieves last mmap position in a fresh start
void escreverRegisto() {
  // start i as -1 only at the beggining  so we can call the get_last_valid_id function to restore the id to the corred position
  static int id = -1;
  if (id == -1)
  {
    id = get_last_valid_id(pa);
  }
  if (id == -1)
  {
    printf("Erro ao restaurar registo");
  }

  pa[id].t = registo.t;
  pa[id].s = registo.s;
  pa[id].temperatura = registo.temperatura;
  
  id++;
  if (id >= NREG)
  {
    id = 0;
  }
}

//receive from sismon queue
void* recebe_dados() {
  
  mqd_t mq;
  
  // Open queue
  mq = mq_open(REGQ, O_RDONLY);
  if (mq == -1) {
    perror("Error mq_open");
    exit(1);
  }

  while (!sigterm_signal)
  {
    // Receive struct from sismon
    if (mq_receive(mq, (char*)&registo, MAX_MSG_SIZE + 1, NULL) == -1) {
        perror("mq_receive");
        pthread_exit(NULL);
      }
    escreverRegisto();
  }

  mq_close(mq);
  return NULL;
}

// here we initialize the socket for intuti communication, initialize the thread for sismon queue, initialize the mmap for the queue and receive inputs from intuti
int main() {   
  
  pthread_t recebe_dados_sismon;
  regsocket regsoc;
  struct timeval timeout = {TIMEOUT_SEC, 0};
  ssize_t msg_status;
  char command[WORDSIZE];
  int mfd;

  //Create socket
  create_reghist_socket(&regsoc);

  //Create signal
  signal(SIGTERM, sighand);

  // Set timeout option for recvfrom
  if (setsockopt(regsoc.sd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }

  // Create Mmap
  if ((mfd = open(DADOS, O_RDWR | O_CREAT, 0666)) < 0)
  { 
    perror("Erro a criar ficheiro");
    exit(-1);
  }
  //Set Mmap size
  if (ftruncate(mfd, NREG*sizeof(reg_t)) < 0)
  { 
    perror("Erro no ftruncate");
    exit(-1);
  }

  //Map the file
  if ((pa=mmap(NULL, NREG*sizeof(reg_t), PROT_READ|PROT_WRITE, MAP_SHARED, mfd, 0)) < (reg_t *)0) {
    perror("Erro em mmap");
    exit(-1);
  }
       
  // Create thread
  if (pthread_create(&recebe_dados_sismon, NULL, recebe_dados, (void *)&mfd) != 0) {
    perror("Error thread\n");
    return 1;
  }
    
  //here we receive commands from intuti
  while (!sigterm_signal) 
  {
    // msg_status stores recvfrom value to check if it was succesful or not
    msg_status = recvfrom(regsoc.sd, command, sizeof(command), 0, (struct sockaddr *)&regsoc.from, &regsoc.fromlen);
    
    //if msg_status < 0, then an error ocurred, now we check if the error is related to the timeout, we dont want sismon to keep printing timeout so we just commented it
    if (msg_status < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      //printf("timeout\n");
    } else if (msg_status < 0) {
      perror("Erro no recvfrom:");
    } else {  
      handle_commands(pa, command, regsoc);
    }
  }

  pthread_join(recebe_dados_sismon, NULL);

  return 0;
}