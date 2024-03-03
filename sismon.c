#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define NS 3 /* numero de sectores a controlar */
#define PSEN 5 /* perıodo inicial dos processos sensores (em segundos) */
#define PACT 8 /* perıodo inicial dos processos actuadores (segundos) */
#define PAMB 10 /* perıodo inicial dos processos ambiente (em segundos) */
#define TINI 25 /* temperatura inicial dos sectores (em graus) */
#define TMIN -100 /* temperatura minima inicial para alarme (em graus) */
#define TMAX +100 /* temperatura maxima inicial para alarme (em graus) */
#define NCICL 12 /* numero de ciclos para alternancia ambiente */
#define NT 3 

int TEMP = TINI;
int tmanip;

/*typedef struct Threadinputs;
{
    int TEMP = TINI;
    int tmanip;
} Threadinput
*/
void *thread_sen (void *TEMP)
{
    int *senbuf;
    senbuf = (int*)TEMP;
    while(1)
    {
        sleep(1);
  if (*senbuf > TMAX)
    {
        printf("ABOVE MAX TEMPERATURE\n");
    }

  if (*senbuf < TMIN)
    {
        printf("BELOW MINIMUM TEMPERATURE\n");
    }

   printf("CURRENT TEMP: %i\n", *senbuf);  
    }
}

void *thread_act (void *TEMP)
{
    int tact;
    int *actbuf;
    actbuf = (int*)TEMP;

    while(1)
    {
         sleep(1);

  if (tmanip > 0)
    {
      tact = 1;  
    }
  else if (tmanip < 0)
    {
      tact = -1;  
    }
  else
    {
      tact = 0;  
    }

   *actbuf += tact; 
   }
}

void *thread_amb (void *TEMP)
{
 int *ambbuf;
 ambbuf = (int*)TEMP;
 int tfixamb;
 int tvaramb;   

    while (1)
    {
      sleep(1);  
      tvaramb = rand() % 2;
      printf("%i\n", tvaramb);
      
    if (tvaramb == 1)
    {
        tvaramb = 1;
    }

    else
    {
        tvaramb = -1;
    }

 /*if ()
    {
     tfixamb = 1;       
    }
 else 
    {
     tfixamb = -1;   
    }
*/

 *ambbuf += tfixamb + tvaramb;
    }
}

void main ()
{
    pthread_t  threads[NT];
  //  Threadinput threadinput;
    
    if (pthread_create(&threads[0], NULL, thread_sen, (void*)&TEMP) != 0) {
        printf("Erro a criar thread sensor"); }
    if (pthread_create(&threads[1], NULL, thread_act, (void*)&TEMP) != 0) {
        printf("Erro a criar thread atuador"); }
    if (pthread_create(&threads[2], NULL, thread_amb, (void*)&TEMP) != 0) {
        printf("Erro a criar thread ambiente"); }

    while (1)
    { 
       scanf("%i", &tmanip);
    }
    
    

}


