#include "naosei.h"

int main ()
{   
  threadinput *threadinput;
  pthread_t  threads[NT];
    
  if (pthread_create(&threads[0], NULL, thread_sen, (void*)&threadinput) != 0) {
      printf("Erro a criar thread sensor"); }
  if (pthread_create(&threads[1], NULL, thread_act, (void*)&threadinput) != 0) {
      printf("Erro a criar thread atuador"); }
  if (pthread_create(&threads[2], NULL, thread_amb, (void*)&threadinput) != 0) {
      printf("Erro a criar thread ambiente"); }

  while (1)
  { 
    scanf("%i", &threadinput->tmanip);
  }

  return 0;
}






