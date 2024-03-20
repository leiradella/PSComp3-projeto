#ifndef REGHIST_H
#define REGHIST_H

#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include <time.h>

#define REGQ "/REGQ"
#define MAX_MSG_SIZE sizeof(reg_t)

typedef struct timespec timespec_t;

typedef struct {
    timespec_t t;
    int temperatura;
    int s;
} reg_t;


#endif
