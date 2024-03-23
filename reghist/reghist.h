#ifndef REGHIST_H
#define REGHIST_H

#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define REGQ "/REGQ"                        /* nome do registo historico (queue) */
#define MAX_MSG_SIZE sizeof(reg_t)
#define NREG 200                            /* numero maximo de registos no ficheiro */
#define DADOS "HISTORICO.LOG"               /* ficheiro com registo historico */

typedef struct timespec timespec_t;

typedef struct {
    timespec_t t;
    int temperatura;
    int s;
} reg_t;


#endif
