#include "reghist_commands.h"
#include "reghist.h"
#include <errno.h>
#include <stdio.h>

#define MAXWORDS 50

void trh(regsocket soc)
{
    char buf[WORDSIZE];

    snprintf(buf, WORDSIZE, "Reghist closing...");
    if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");\
    exit(0);
}

void lreg(reg_t *pa, char **args, int argc, regsocket soc)
{
    char buf[WORDSIZE];
    int i, id1, id2, sector = -1;
    time_t tspec1, tspec2, lspec = -1, hspec =-1;
    struct tm *date;

    //  get the sector
    sector = (int)strtol(args[1], NULL, 10);
    if (sector == 0 && errno == EINVAL)
    {
        snprintf(buf, WORDSIZE, "Reghist: Setor invalido");
        if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
        return;
    }

    tspec1 = date_to_seconds(args[2], args[3]);

    switch (argc) {
        //if its 4, then client didnt specify t2
        case 4:
            // searches all timespecs in mmap for the timespec closest to t1 and store the one closest to t1 inside
            // the interval. also stores the most recent entry in id2.
            for (i = 0; i < NREG; i++)
            {
                if(timespec_to_seconds(pa[i].t) >= tspec1 && (lspec == -1 || timespec_to_seconds(pa[i].t) < lspec))
                {
                    lspec = timespec_to_seconds(pa[i].t);
                    id1 = i;
                }
                if (timespec_to_seconds(pa[i].t) > hspec)
                {
                    hspec = timespec_to_seconds(pa[i].t);
                    id2 = i;
                }
            }
            //if there are no records that come after the specified t1 date, lspec will be -1
            if (lspec == -1)
            {
                snprintf(buf, WORDSIZE, "Reghist: There are no records after the specified t1 date, try an older date instead.");
                if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");\
                return;
            }

            if (id1 > id2)
            {
                //this for goes from id1 to the bottom of mmap
                for (id1 = id1; id1 <= NREG; id1++)
                {
                    //checks if current entry s (sector) = sector argument value or if sector = 0 (all sectors)
                    if (pa[id1].s == sector || sector == 0)
                    {
                        date = seconds_to_date(pa[id1].t);
                        snprintf(buf, WORDSIZE, "Timestamp: %d/%d/%d %d:%d:%d\nSetor: %d\nTemperatura: %d\n", date->tm_mday, date->tm_mon, date->tm_year, date->tm_hour, date->tm_min, date->tm_sec, pa[id1].s, pa[id1].temperatura);
                        if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
                    }
                }
                for (id1 = 0; id1 <= id2; id1++)
                {
                    //checks if current entry s (sector) = sector argument value or if sector = 0 (all sectors)
                    if (pa[id1].s == sector || sector == 0)
                    {
                        date = seconds_to_date(pa[id1].t);
                        snprintf(buf, WORDSIZE, "Timestamp: %d/%d/%d %d:%d:%d\nSetor: %d\nTemperatura: %d\n", date->tm_mday, date->tm_mon, date->tm_year, date->tm_hour, date->tm_min, date->tm_sec, pa[id1].s, pa[id1].temperatura);
                        if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
                    }
                }
            } else {
                // if id2 > id1 just list all positions between them
                for (id1 = id1; id1 <= id2; id1++)
                {
                    //checks if current entry s (sector) = sector argument value or if sector = 0 (all sectors)
                    if (pa[id1].s == sector || sector == 0)
                    {
                        date = seconds_to_date(pa[id1].t);
                        snprintf(buf, WORDSIZE, "Timestamp: %d/%d/%d %d:%d:%d\nSetor: %d\nTemperatura: %d\n", date->tm_mday, date->tm_mon, date->tm_year, date->tm_hour, date->tm_min, date->tm_sec, pa[id1].s, pa[id1].temperatura);
                        if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
                    }
                }
            }
            break;
        //if its 6, then cliedate_timent specified t2
        case 6:
            tspec2 = date_to_seconds(args[4], args[5]);
            // searches all timespecs in mmap for the timespecs closest to t1 and t2
            // by the end, id1 will have the entry number of the first entry and id2 will have the entry number of the last entry
            for (i = 0; i < NREG; i++)
            {
                if(timespec_to_seconds(pa[i].t) >= tspec1 && (lspec == -1 || timespec_to_seconds(pa[i].t) < lspec))
                {
                    lspec = timespec_to_seconds(pa[i].t);
                    id1 = i;
                }
                if(timespec_to_seconds(pa[i].t) <= tspec2 && (hspec == -1 || timespec_to_seconds(pa[i].t) > hspec))
                {
                    hspec = timespec_to_seconds(pa[i].t);
                    id2 = i;
                }
            }
            //if there are no records that come after the specified t1 date, lspec will be -1
            if (lspec == -1)
            {
                snprintf(buf, WORDSIZE, "Reghist: There are no records after the specified t1 date, try an older date instead.");
                if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
                return;
            }
            //if there are no records that come before the specified t2 date, hspec will be -1
            if (hspec == -1)
            {
                snprintf(buf, WORDSIZE, "Reghist: There are no records befdore the specified t2 date, try an older date instead.");
                if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
                return;
            }

            // if lspec and hspec passed the tests, then its time to send the valid log entries to the user
            // if id1 is greater than id2, it means the span of entries we want reaches the end of mmap and goes back to the start 
            if (id1 > id2)
            {
                //this for goes from id1 to the bottom of mmap
                for (id1 = id1; id1 <= NREG; id1++)
                {
                    //checks if current entry s (sector) = sector argument value or if sector = 0 (all sectors)
                    if (pa[id1].s == sector || sector == 0)
                    {
                        date = seconds_to_date(pa[id1].t);
                        snprintf(buf, WORDSIZE, "Timestamp: %d/%d/%d %d:%d:%d\nSetor: %d\nTemperatura: %d\n", date->tm_mday, date->tm_mon, date->tm_year, date->tm_hour, date->tm_min, date->tm_sec, pa[id1].s, pa[id1].temperatura);
                        if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
                    }
                }
                //this goes from start of mmap until id2
                for (id1 = 0; id1 <= id2; id1++)
                {
                    //checks if current entry s (sector) = sector argument value or if sector = 0 (all sectors)
                    if (pa[id1].s == sector || sector == 0)
                    {
                        date = seconds_to_date(pa[id1].t);
                        snprintf(buf, WORDSIZE, "Timestamp: %d/%d/%d %d:%d:%d\nSetor: %d\nTemperatura: %d\n", date->tm_mday, date->tm_mon, date->tm_year, date->tm_hour, date->tm_min, date->tm_sec, pa[id1].s, pa[id1].temperatura);
                        if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
                    }
                }
            } else {
                // if id2 > id1 just list all positions between them
                for (id1 = id1; id1 <= id2; id1++)
                {
                    //checks if current entry s (sector) = sector argument value or if sector = 0 (all sectors)
                    if (pa[id1].s == sector || sector == 0)
                    {
                        date = seconds_to_date(pa[id1].t);
                        snprintf(buf, WORDSIZE, "Timestamp: %d/%d/%d %d:%d:%d\nSetor: %d\nTemperatura: %d\n", date->tm_mday, date->tm_mon, date->tm_year, date->tm_hour, date->tm_min, date->tm_sec, pa[id1].s, pa[id1].temperatura);
                        if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
                    }
                }
            }
            break;
        default:
            snprintf(buf, WORDSIZE, "Reghist: lreg invalid number of arguments = %d (must be 3 or 5)", argc-1);
            if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
            break;
    }
}

void handle_commands(reg_t *pa, char *command, regsocket soc)
{
    char *token, **args;
    int id, argc = 0;

    // make space for args
    args = (char **)malloc(MAXWORDS*sizeof(char*));
    if (args == NULL){
      printf("Erro ao alocar memoria\n");
      return;
    }

    //get first word of string (the command)
    token = strtok(command, " ");
    
    //put all words of commands in args
    while (token != NULL)
    {
        //make space for current word
        args[argc] = (char *)malloc(WORDSIZE*sizeof(char));
        if (args == NULL){
            printf("Erro ao alocar memoria\n");
            return;
        }
        strcpy(args[argc], token);
        token = strtok(NULL, " ");
        argc++;
    }
    
    id = (int)strtol(args[0], NULL, 10);
    //if the first word inst a valid id for a reghist command, then it must be a string command
    if (id != LREG && id != TRH)
    {
        handler2(pa, args, argc, soc);
    } else {
        handler1(pa, args, id, argc, soc);
    }
    
    //free mallocs
    for(int i = 0; i < argc; i++)
    {
        free(args[i]);
    }
    free(args);
}

void handler1(reg_t *pa, char **args, int id, int argc,regsocket soc)
{
    char buf[WORDSIZE];

    switch (id){
        case TRH:
            trh(soc);
            break;
        case LREG:
            lreg(pa, args, argc, soc);
            break;
        default:
            snprintf(buf, WORDSIZE, "Reghist: Invalid command id (mode 1)");
            if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
    }
        
}

void handler2(reg_t *pa, char **args, int argc, regsocket soc)
{
    char buf[WORDSIZE];

    switch (argc) {
        case 1:
            if (strcmp(args[0], "trh") == 0)
            {
                trh(soc);
            }
            break;
        case 3:
            if (strcmp(args[0], "lreg") == 0)
            {
                lreg(pa, args, argc, soc);
            }
            break;
        case 4:
            if (strcmp(args[0], "lreg") == 0)
            {
                lreg(pa, args, argc, soc);
            }
            break;
        default:
            snprintf(buf, WORDSIZE, "Reghist: Invalid command (mode 2)");
            if (sendto(soc.sd, buf, strlen(buf)+1, 0, (struct sockaddr *)&soc.from, soc.fromlen) < 0) perror("CLI: Erro no sendto");
    }
}

time_t date_to_seconds(char *date, char *time)
{
    struct tm date_time;

    date_time.tm_wday = 0;
    date_time.tm_yday = 0;
    date_time.tm_isdst = 0;
    
    if (date == NULL) {
        printf("date_to_seconds: date is NULL");
        return -1;
    }
    if (time == NULL) {
        printf("date_to_seconds: time is NULL");
        return -1;
    }

    //we put dd/mm/aaaa and hh:mm:ss into the variables
    sscanf(date, "%d/%d/%d", &date_time.tm_mday, &date_time.tm_mon, &date_time.tm_year);
    sscanf(time, "%d:%d:%d", &date_time.tm_hour, &date_time.tm_min, &date_time.tm_sec);
    // honestly i have no idea why it gives back the exact same time but one hour less, might be DST. idk and honestly idc. this fixes the issue
    // we will see if it changes march 31st 💀
    // NOW IT WORKS 5 MINUTES AFTER I DID THIS HOW IM COMMENTING THIS I STG IF IT GOES 1 HOUR BACK AGAIN
    //date_time.tm_hour++;

    return mktime(&date_time);    
}

time_t timespec_to_seconds(struct timespec spec)
{
    return spec.tv_sec;
}

struct tm * seconds_to_date(struct timespec tspec)
{
    time_t seconds = tspec.tv_sec; 
    return gmtime(&seconds);;
}

int get_last_valid_id(reg_t *pa)
{
    int i, id;
    time_t hvalue = -1;

    //go through entire mmap and find entry with the biggest timespec value, and return the id of that entry
    for (i = 0; i < NREG; i++)
    {
        if(pa[i].t.tv_sec >= hvalue)
        {
            hvalue = pa[i].t.tv_sec;
            id = i;
        }
    }
    // if historico doesnt have any logs, then all positions will be zero and id will be stored as 199.
    // because of this, we added a check, if by the end of going through the entire mmap hvalue is still 0, then it means
    // the log is new, so we set id to 1.
    if (hvalue == 0)
    {
        id = 0;
    } else if (hvalue != -1) {
        //this is just because id stores the newest entry, so we have to add one to start writing. (if hvalue = -1, then mmap is tweaking)
        id++;
    }

    return id;
}