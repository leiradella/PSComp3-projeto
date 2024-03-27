/***************************************************************************
| File:   reghist.h
|
| Group:  4
|
| Autors: Miguel Fernades  103024
|         Gonçalo Antunes  103524
|         Lucas Leiradella 103566
|
| Data:  Mar 2024
***************************************************************************/
#ifndef REGHIST_H
#define REGHIST_H

#include <time.h>

#define REGQ "/REGQ"                        
#define MAX_MSG_SIZE sizeof(reg_t)
#define NREG 200                            
#define WORDSIZE 200
#define DADOS "../reghist/HISTORICO.LOG"               

typedef struct timespec timespec_t;

typedef struct {
    timespec_t t;
    int temperatura;
    int s;
} reg_t;

#endif
