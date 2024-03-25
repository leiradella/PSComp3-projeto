#ifndef REGHIST_H
#define REGHIST_H

#include <time.h>

#define REGQ "/REGQ"                        /* nome do registo historico (queue) */
#define MAX_MSG_SIZE sizeof(reg_t)
#define NREG 200                            /* numero maximo de registos no ficheiro */
#define WORDSIZE 200
#define DADOS "HISTORICO.LOG"               /* ficheiro com registo historico */

typedef struct timespec timespec_t;

typedef struct {
    timespec_t t;
    int temperatura;
    int s;
} reg_t;

#endif
