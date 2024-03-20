#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include "reghist.h"

void* recebe_dados(void *mq) {
  
  mqd_t *a = (mqd_t*)mq;
  reg_t registo;
    
    while (1) 
    {
      // Recebendo a estrutura de dados do servidor
      if (mq_receive(*a, (char*)&registo, sizeof(reg_t), NULL) == -1) {
        perror("mq_receive");
        pthread_exit(NULL);
      }

      // Exibindo a estrutura de dados recebida
      printf("Registro recebido do servidor:\n");
      printf("Timestamp: %ld.%ld\n", registo.t.tv_sec, registo.t.tv_nsec);
      printf("Setor: %d\n", registo.s);
      printf("Temperatura: %d\n", registo.temperatura);
      printf("\n \n \n \n \n");
      }
}

int main() {   
  
  mqd_t mq;
  pthread_t recebe_dados_sismon;

    // Open queue
    mq = mq_open(REGQ, O_RDONLY);
      if (mq == -1) {
        perror("Error mq_open");
        exit(1);
      }
    
    // Create thread
    if (pthread_create(&recebe_dados_sismon, NULL, recebe_dados, (void *)&mq) != 0) {
      perror("Error thread\n");
      return 1;
    }

    // Socket

    // MMAP

    while (1) 
    {
    
    
    
    
    
    
    
    
    
    
    }

    return 0;
}