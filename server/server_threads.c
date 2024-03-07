#include "server_threads.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void* thread_sen(void *threadinput1) {
    
  threadinput *senbuf = (threadinput*)threadinput1;

  while(1)
  {
    sleep(PSEN);
    if (senbuf->TEMP > TMAX) 
    {  
      printf("ABOVE MAX TEMPERATURE\n"); 
    }

    if (senbuf->TEMP < TMIN) 
    {
      printf("BELOW MINIMUM TEMPERATURE\n"); 
    }

    printf("CURRENT TEMP: %i\n",senbuf->TEMP); 
  }
}

void* thread_act (void *threadinput1)
{
  int tact;
  threadinput *actbuf = (threadinput*)threadinput1;
  
  while(1)
  {
    sleep(PACT);

    if (actbuf->tmanip > 0)
    {
      tact = 1;  
    }
    else if (actbuf->tmanip < 0)
    {
      tact = -1;  
    }
    else
    {
      tact = 0;  
    }
    actbuf->TEMP += tact; 
  }
}

void* thread_amb (void *threadinput1)
{
 threadinput *ambbuf = (threadinput*)threadinput1;

 int tfixamb;
 int tvaramb;
 int ambcycle = 0;   

  while (1)
  {
    sleep(PAMB);  
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
    ambbuf->TEMP += tfixamb + tvaramb;
    ambcycle++;
  }
} 
