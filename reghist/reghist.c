#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <time.h>

#define REGQ "/REGQ"
#define MAX_MSG_SIZE sizeof(reg_t)

typedef struct timespec timespec_t;

typedef struct {
    timespec_t t;
    int temperatura;
    int s;
} reg_t;

int main ()
{   
  mqd_t mq;
  reg_t registro;

    // Abrindo a fila de mensagens
    mq = mq_open(REGQ, O_RDONLY);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }
  while (1)
  {
    // Recebendo a estrutura de dados do servidor
    if (mq_receive(mq, (char*)&registro, MAX_MSG_SIZE + 1, NULL) == -1) {
        perror("mq_receive");
        exit(1);
    }

    // Exibindo a estrutura de dados recebida
    printf("Registro recebido do servidor:\n");
    printf("Timestamp: %ld.%ld\n", registro.t.tv_sec, registro.t.tv_nsec);
    printf("Setor: %d\n", registro.s);
    printf("Temperatura: %d\n", registro.temperatura);
    printf("\n \n \n \n \n");

  }
  return 0;
}
