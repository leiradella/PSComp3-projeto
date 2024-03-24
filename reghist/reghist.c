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

#define TIMEOUT_SEC 5

int sigterm_signal = 0;
reg_t registo;

// Signal handler function
void sighand(int signum) {
  printf("Received SIGTERM signal (%d). Reghist Exiting...\n", signum);
  sigterm_signal = 1;
}

void imprimirDataHora(long int segundos, long int nanosegundos) {
  struct tm *infoTempo;
  char buffer[WORDSIZE];

  // Converter segundos para a estrutura tm
  time_t tempo = (time_t)segundos;
  infoTempo = localtime(&tempo);

  // Arredondar os segundos
  long int segundosArredondados = infoTempo->tm_sec + ((nanosegundos / 1000000) >= 500 ? 1 : 0);

  // Formatar a data e hora
  snprintf(buffer, sizeof(buffer), "Timestamp: %02d/%02d/%04d %02d:%02d:%02ld",
      infoTempo->tm_mday, infoTempo->tm_mon + 1, infoTempo->tm_year + 1900,
      infoTempo->tm_hour, infoTempo->tm_min, segundosArredondados);

  // Imprimir a data e hora formatada
  
}

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
    // Recebendo a estrutura de dados do servidor
    if (mq_receive(mq, (char*)&registo, MAX_MSG_SIZE + 1, NULL) == -1) {
        perror("mq_receive");
        pthread_exit(NULL);
      }

    imprimirDataHora(registo.t.tv_sec, registo.t.tv_nsec);
    printf("Setor: %d\n", registo.s);
    printf("Temperatura: %d\n", registo.temperatura);
  }

  mq_close(mq);
  return NULL;
}

int main() {   
  
  pthread_t recebe_dados_sismon;
  regsocket regsoc;
  struct timeval timeout = {TIMEOUT_SEC, 0};
  ssize_t msg_status;
  char command[WORDSIZE], *pa;
  int mfd;


  //create socket
  create_reghist_socket(&regsoc);

  // Set timeout option for recvfrom
  if (setsockopt(regsoc.sd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }

  // Create Mmap
  if ((mfd = open(DADOS, O_RDWR | O_CREAT, 0666)) < 0)
  { 
    /* abrir / criar ficheiro */
    perror("Erro a criar ficheiro");
    exit(-1);
  }
  if (ftruncate(mfd, NREG*sizeof(reg_t)) < 0)
  { 
    /* definir tamanho do ficheiro */
    perror("Erro no ftruncate");
    exit(-1);
  }

  /* mapear ficheiro */
  if ((pa=mmap(NULL, NREG*sizeof(reg_t), PROT_READ|PROT_WRITE, MAP_SHARED, mfd, 0)) < (char *)0) {
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
      handle_commands(pa, registo, command, regsoc);
    }
  }

  pthread_join(recebe_dados_sismon, NULL);

  return 0;
}