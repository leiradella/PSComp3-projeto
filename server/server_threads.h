/***************************************************************************
| File:   server_threads.h
|
| Group:  4
|
| Autors: Miguel Fernades  103024
|         Gonçalo Antunes  103524
|         Lucas Leiradella 103566
|
| Data:  Mar 2024
***************************************************************************/
#ifndef SISMON_H_
#define SISMON_H_

#include <pthread.h>
#include <mqueue.h>

#define NS 3                     // number of sectors to control
#define PSEN 5                   // initial period of sensor processes (seconds)
#define PACT 8                   // initial period of actuating processes (seconds) 
#define PAMB 10                  // initial period of ambient processes (seconds) 
#define TINI 25                  // initial temperature of the sectors (in degrees)
#define TMIN -100                // minimum initial temperature for alarm (in degrees) 
#define TMAX +100                // maximum initial temperature for alarm (in degrees) 
#define NCICL 12                 // number of cycles for ambient alternation
#define NT 3                     // number of threads
#define YEAR_CONVERT 59960822400 // sum this to timespec.tv_sec so when you use gmtime it actually gives you the correct date without having to sum 1900 years and 1 month


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
