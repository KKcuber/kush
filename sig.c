#include "headers.h"

int getJobIndex_givenJobNum(int jobNum)
{
    for (int i = 0; i < bjob_count; i++)
    {
        if (job_arr[i].job_num == jobNum && job_arr[i].pid > 0)
        {
            return i;
        }
    }
    return -1;
}

int getJobIndex_givenPid(int pid)
{
    for (int i = 0; i < bjob_count; i++)
    {
        if (job_arr[i].pid == pid)
        {
            return i;
        }
    }
    return -1;
}

void removeJob2(int pid)
{
    int index = getJobIndex_givenPid(pid);
    if (index == -1)
    {
        return;
    }
    job_arr[index].pid = -69696969;
    job_num_available[job_arr[index].job_num] = 0;
}

// Takes the job number (assigned by your shell) of a running job and sends the signal corresponding to signal number to that process
void sig(int numTokens)
{
    if(numTokens != 3)
    {
        printf("Invalid number of arguments\n");
        return;
    }
    int jobNum = atoi(token[1]);
    int sigNum = atoi(token[2]);
    int jobIndex = getJobIndex_givenJobNum(jobNum);
    if (jobIndex == -1)
    {
        printf("No such job");
        return;
    }
    pid_t pid = job_arr[jobIndex].pid;
    kill(pid, sigNum);
}

void bg(int numTokens)
{
    if(numTokens != 2)
    {
        printf("Invalid number of arguments\n");
        return;
    }
    int jobNum = atoi(token[1]);
    int jobIndex = getJobIndex_givenJobNum(jobNum);
    if (jobIndex == -1)
    {
        printf("No such job");
        return;
    }
    pid_t pid = job_arr[jobIndex].pid;
    kill(pid, SIGCONT);
}

void fg(int numTokens)
{
    if(numTokens != 2)
    {
        printf("Invalid number of arguments\n");
        return;
    }
    int jobNum = atoi(token[1]);
    int jobIndex = getJobIndex_givenJobNum(jobNum);
    if (jobIndex == -1)
    {
        printf("No such job\n");
        return;
    }
    pid_t pid = job_arr[jobIndex].pid;
    curr_foregound_job.job_num = 0;
    curr_foregound_job.pid = pid;
    strcpy(curr_foregound_job.name,job_arr[jobIndex].name);
    // signal(SIGTTOU, SIG_IGN);
    // signal(SIGTTIN, SIG_IGN);
    kill(pid, SIGCONT);
    int status=0;
    waitpid(pid,&status,WUNTRACED);
    // signal(SIGTTOU, SIG_DFL);
    // signal(SIGTTIN, SIG_DFL);
    if(!WIFSTOPPED(status))
    {
        removeJob2(pid);
    }
    curr_foregound_job.pid = -1;
    curr_foregound_job.name[0] = '\0';
}