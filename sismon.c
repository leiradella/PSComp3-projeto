#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct Threadinputs
     {
    int TEMP;
    int tmanip;
     };

     struct Threadinputs threadinput;
    

void* thread_sen (void *threadinput)
{
    struct Threadinputs *senbuf = (struct Threadinputs*)threadinput;

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

   printf("CURRENT TEMP: %i\n", senbuf->TEMP);  
    }
}

void* thread_act (void *threadinput)
{
    int tact;
    struct Threadinputs *actbuf = (struct Threadinputs*)threadinput;
    
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

void* thread_amb (void *threadinput)
{
 struct Threadinputs *ambbuf = (struct Threadinputs*)threadinput;

 int tfixamb;
 int tvaramb;
 int ambcycle;   

    while (1)
    {
      sleep(PAMB);  
      tvaramb = rand() % 2;
      
    if (tvaramb == 1)
    {
        tvaramb = 1;
    }

    else
    {
        tvaramb = -1;
    }

    if (ambcycle%NCICL == 0)
    {
     tfixamb = 1;       
    }
    else 
    {
     tfixamb = -1;
     if (ambcycle == 2*NCICL)
        {
            ambcycle = 0;
        }   
    }


 ambbuf->TEMP += tfixamb + tvaramb;
 ambcycle++;
    }
}


void main()
{   
     threadinput.TEMP = TINI;
    pthread_t  threads[NT];
    
    if (pthread_create(&threads[0], NULL, thread_sen, (void*)&threadinput) != 0) {
        printf("Erro a criar thread sensor"); }
    if (pthread_create(&threads[1], NULL, thread_act, (void*)&threadinput) != 0) {
        printf("Erro a criar thread atuador"); }
    if (pthread_create(&threads[2], NULL, thread_amb, (void*)&threadinput) != 0) {
        printf("Erro a criar thread ambiente"); }

    while (1)
    { 
       scanf("%i", &threadinput.tmanip);
    }

}



