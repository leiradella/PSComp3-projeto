#include "server_threads.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int tmin = TMIN;
int tmax = TMAX;
time_t start1, end1, start2,end2,start3,end3;

void* thread_sen(void *threadinput) {
    
  struct Threadinputs *senbuf = (struct Threadinputs*)threadinput;

  while(1)
  {
    if (senbuf->psen != 0)
    {
      time(&start1);
      do time(&end1); while(end1 <= start1 + senbuf->psen);

      if (senbuf->TEMP > tmax) 
      {  
        printf("ABOVE MAX TEMPERATURE\a\n"); 
      }

      if (senbuf->TEMP < tmin) 
      {
        printf("BELOW MINIMUM TEMPERATURE\a\n"); 
      }
    }
    start1 = end1;
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
      time(&start2);
      do time(&end2); while(end2 <= start2 + actbuf->pact);

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
    start2 = end2;
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
      time(&start3);
      do time(&end3); while(end3 <= start3 + ambbuf->pamb);

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
    start3 = end3;
  }
} 
