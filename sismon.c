#include "naosei.h"

int main ()
{   
  threadinput *threadinput1;
  pthread_t  threads[NT];
    
  if (pthread_create(&threads[0], NULL, thread_sen, (void*)&threadinput1) != 0) {
      printf("Erro a criar thread sensor"); }
  if (pthread_create(&threads[1], NULL, thread_act, (void*)&threadinput1) != 0) {
      printf("Erro a criar thread atuador"); }
  if (pthread_create(&threads[2], NULL, thread_amb, (void*)&threadinput1) != 0) {
      printf("Erro a criar thread ambiente"); }

  while (1)
  { 
    scanf("%i", &threadinput1->tmanip);
  }

  return 0;
}






