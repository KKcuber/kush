#include "headers.h"

void ctrl_c()
{
    pid_t curpid = getpid();
    if(curpid == shellpid)
    {
        printf("\n");
        prompt();
        fflush(stdout);
    }
    else
    {
        kill(curpid, SIGINT);
    }
}

void ctrl_z()
{
    pid_t curpid = getpid();
    if(curpid == shellpid)
    {
        printf("\n");
        prompt();
        fflush(stdout);
    }
    else
    {
        kill(curpid, SIGTSTP);
        job_arr[bjob_count].pid = curpid;
        strcpy(job_arr[bjob_count].name, token[0]);
        bjob_count++;
    }
}