#include "server_threads.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int tmin = TMIN;
int tmax = TMAX;

void* thread_sen(void *threadinput) {
    
  struct Threadinputs *senbuf = (struct Threadinputs*)threadinput;

  while(1)
  {
    if (senbuf->psen != 0)
    {
      sleep(senbuf->psen)
      if (senbuf->TEMP > tmax) 
      {  
        printf("ABOVE MAX TEMPERATURE\a\n"); 
      }

      if (senbuf->TEMP < tmin) 
      {
        printf("BELOW MINIMUM TEMPERATURE\a\n"); 
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
      actbuf->TEMP += tact; 
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
      ambbuf->TEMP += tfixamb + tvaramb;
      ambcycle++;
    }
  }
} 
