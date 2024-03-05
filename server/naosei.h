#ifndef SISMON_H_
#define SISMON_H_

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NS 3 /* numero de sectores a controlar */
#define PSEN 5 /* perıodo inicial dos processos sensores (em segundos) */
#define PACT 8 /* perıodo inicial dos processos actuadores (segundos) */
#define PAMB 10 /* perıodo inicial dos processos ambiente (em segundos) */
#define TINI 25 /* temperatura inicial dos sectores (em graus) */
#define TMIN -100 /* temperatura minima inicial para alarme (em graus) */
#define TMAX +100 /* temperatura maxima inicial para alarme (em graus) */
#define NCICL 12 /* numero de ciclos para alternancia ambiente */
#define NT 3   /*numero de threads*/

typedef struct Threadinputs {
    int TEMP;
    int tmanip;
} threadinput;

void* thread_sen (void *threadinput1);

void* thread_act (void *threadinput1);

void* thread_amb (void *threadinput1);

#endif