#include "server_threads.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int tmin = TMIN;
int tmax = TMAX;
pthread_mutex_t mutex;


void* thread_sen(void *threadinput) {
    
  struct Threadinputs *senbuf = (struct Threadinputs*)threadinput;
	long double timenothefunction;
	clock_t t;
  while(1)
  {
  t = clock();
    if (senbuf->psen != 0)
    {
      if (senbuf->TEMP > tmax) 
      {  
        printf("ABOVE MAX TEMPERATURE\a\n"); 
      }

      if (senbuf->TEMP < tmin) 
      {
        printf("BELOW MINIMUM TEMPERATURE\a\n"); 
      }
     t = clock() - t; 
     timenothefunction = ((double)t)/CLOCKS_PER_SEC;
     sleep(senbuf->psen - timenothefunction);
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
    if (actbuf->pact != 0)
    {
     
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
    t = clock() - t; 
   time = ((double)t)/CLOCKS_PER_SEC;
   sleep(actbuf->pact - time);  
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
    if (ambbuf->pamb != 0)
    {
   
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
    t = clock() - t; 
   time = ((double)t)/CLOCKS_PER_SEC;
   sleep(ambbuf->pamb - time);  
  }
} 
