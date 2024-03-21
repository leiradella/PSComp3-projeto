#ifndef SISMON_H_
#define SISMON_H_

#include <pthread.h>
#include <mqueue.h>

#define NS 3 /* numero de sectores a controlar */
#define PSEN 5 /* perıodo inicial dos processos sensores (em segundos) */
#define PACT 8 /* perıodo inicial dos processos actuadores (segundos) */
#define PAMB 10 /* perıodo inicial dos processos ambiente (em segundos) */
#define TINI 25 /* temperatura inicial dos sectores (em graus) */
#define TMIN -100 /* temperatura minima inicial para alarme (em graus) */
#define TMAX +100 /* temperatura maxima inicial para alarme (em graus) */
#define NCICL 12 /* numero de ciclos para alternancia ambiente */
#define NT 3   /*numero de threads*/

extern int tmin;
extern int tmax;
extern int variavel_controlo_registo;

extern pthread_mutex_t mutex;

typedef struct timespec timespec_t;

typedef struct Threadinputs {
    int TEMP;
    char tmanip;
    int psen;
    int pact;
    int pamb;
    int id;
    time_t regtime;
} thinput;

typedef struct {
    timespec_t t;
    int temperatura;
    int id;
} registo_queue;

void* thread_sen (void *threadinput);

void* thread_act (void *threadinput);

void* thread_amb (void *threadinput);

#endif
