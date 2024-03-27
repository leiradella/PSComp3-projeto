/***************************************************************************
| File:   server_threads.c
|
| Group:  4
|
| Autors: Miguel Fernades  103024
|         Gonçalo Antunes  103524
|         Lucas Leiradella 103566
|
| Data:  Mar 2024
***************************************************************************/
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

void* thread_sen(void *threadinput) {
    
  struct Threadinputs *senbuf = (struct Threadinputs*)threadinput;
	long double timenothefunction;
  static registo_queue dados_registo;
	clock_t t;
  while(1)
  {
  t = clock();
    //check if psen is valid, if 0 then thread does nothing
    if (senbuf->psen != 0)
    {
      //check if temperature thas passed the limits
      if (senbuf->TEMP > tmax) 
      {  
        printf("ABOVE MAX TEMPERATURE\n"); 
      }

      if (senbuf->TEMP < tmin) 
      {
        printf("BELOW MINIMUM TEMPERATURE\n"); 
      }
      // checks if there is a new log to be sent, if this thread runs twice and there is no new log to be sent, it does not get sent
      if (senbuf->id != dados_registo.id && senbuf->TEMP != dados_registo.temperatura && variavel_controlo_registo == 1)
      {     
        clock_gettime(CLOCK_REALTIME, &dados_registo.t);
        dados_registo.t.tv_sec = dados_registo.t.tv_sec + YEAR_CONVERT;
        dados_registo.id = senbuf->id;
        dados_registo.temperatura = senbuf->TEMP;

        // Send struct to reghist
        if (mq_send(mq, (const char*)&dados_registo, MAX_MSG_SIZE, 0) == -1) {
          perror("mq_send");
          exit(1);
        }
      } 
      //here we calculate the time of execution and sleep accordingly to make sure the average period of thread right
      t = clock() - t; 
      timenothefunction = ((double)t)/CLOCKS_PER_SEC;
      sleep((unsigned int)(senbuf->psen - timenothefunction));
      time(&senbuf->regtime);
    }
  }
}

void* thread_act (void *threadinput)
{
  int tact;
  struct Threadinputs *actbuf = (struct Threadinputs*)threadinput;
  long double time;
  clock_t t;
  while(1)
  {
  t = clock();
    //chec if it has valid period
    if (actbuf->pact != 0)
    {
      //checks value of tmanip, which determines if actuators raise, decrease or dont do anything to temperature
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
      //use mutex because temp can be read by server_commands
      pthread_mutex_lock(&mutex);
      actbuf->TEMP += tact; 
      pthread_mutex_unlock(&mutex);
    }
    //here we calculate the time of execution and sleep accordingly to make sure the average period of thread right
    t = clock() - t; 
    time = ((double)t)/CLOCKS_PER_SEC;
    sleep((unsigned int)(actbuf->pact - time));  
  }
}

void* thread_amb (void *threadinput)
{
 struct Threadinputs *ambbuf = (struct Threadinputs*)threadinput;

 int tfixamb;
 int tvaramb;
 int ambcycle = 0;   
 long double time;
 clock_t t;
  while (1)
  {
  t = clock();
    //check for valid period
    if (ambbuf->pamb != 0)
    {
      //use rand to determine tvaramb
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
      //lock because temp can be read by server_commands
      pthread_mutex_lock(&mutex);
      ambbuf->TEMP += tfixamb + tvaramb;
      pthread_mutex_unlock(&mutex);
      ambcycle++;
    }
    //here we calculate the time of execution and sleep accordingly to make sure the average period of thread right
    t = clock() - t; 
   time = ((double)t)/CLOCKS_PER_SEC;
   sleep((unsigned int)(ambbuf->pamb - time));  
  }
} 
