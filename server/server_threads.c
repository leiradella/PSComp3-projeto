#include "server_threads.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_MSG_SIZE sizeof(registo_queue)

int tmin = TMIN;
int tmax = TMAX;
pthread_mutex_t mutex;
extern mqd_t mq;

registo_queue dados_registo;

void* thread_sen(void *threadinput) {
    
  struct Threadinputs *senbuf = (struct Threadinputs*)threadinput;

  while(1)
  {
    if (senbuf->psen != 0)
    {
      sleep(senbuf->psen);
      if (senbuf->TEMP > tmax) 
      {  
        printf("ABOVE MAX TEMPERATURE\a\n"); 
      }

      if (senbuf->TEMP < tmin) 
      {
        printf("BELOW MINIMUM TEMPERATURE\a\n"); 
      }

        if (senbuf->id != dados_registo.id && senbuf->TEMP != dados_registo.temperatura && variavel_controlo_registo == 1)
        {      
          clock_gettime(CLOCK_REALTIME, &dados_registo.t);
          dados_registo.id = senbuf->id;
          dados_registo.temperatura = senbuf->TEMP;

          
          // Enviando a estrutura de dados para o cliente
          if (mq_send(mq, (const char*)&dados_registo, MAX_MSG_SIZE, 0) == -1) {
            perror("mq_send");
            exit(1);
          }
        }  
    }
  }
}

void* thread_act (void *threadinput)
{
  int tact;
  struct Threadinputs *actbuf = (struct Threadinputs*)threadinput;

  while(1)
  {
    if (actbuf->pact != 0)
    {
      sleep(actbuf->pact);
      if (actbuf->tmanip == '+')
      {
        tact = 1;  
      }
      else if (actbuf->tmanip == '-')
      {
        tact = -1;  
      }
      else if (actbuf->tmanip == '0')
      {
        tact = 0;  
      }
      pthread_mutex_lock(&mutex);
      actbuf->TEMP += tact; 
      pthread_mutex_unlock(&mutex);
    }
  }
}

void* thread_amb (void *threadinput)
{
 struct Threadinputs *ambbuf = (struct Threadinputs*)threadinput;

 int tfixamb;
 int tvaramb;
 int ambcycle = 0;   
  while (1)
  {
    if (ambbuf->pamb != 0)
    {
      sleep(ambbuf->pamb);
      tvaramb = rand() % 3 -1;

      if (ambcycle/NCICL == 0)
      {
        tfixamb = 1; 
      }
      else 
      {
      
        tfixamb = -1;
      
        if (ambcycle == 2*NCICL)
        {
        ambcycle = 0;
        tfixamb = 1;
        }   
      }
      pthread_mutex_lock(&mutex);
      ambbuf->TEMP += tfixamb + tvaramb;
      pthread_mutex_unlock(&mutex);
      ambcycle++;
    }
  }
} 
