#include "headers.h"

int getJobIndex(int pid)
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

void removeJob(int pid)
{
    int index = getJobIndex(pid);
    if (index == -1)
    {
        return;
    }
    job_arr[index].pid = -69696969;
    job_num_available[job_arr[index].job_num] = 0;
}

void childHandler()
{
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
    {
        int jobIndex = getJobIndex(pid);
        char name[10000];
        strcpy(name, job_arr[jobIndex].name);
        if (jobIndex == -1)
        {
            continue;
        }
        if (WIFEXITED(status))
        {
            printf("\n%s with pid %d exited normally\n", name, pid);
            removeJob(pid);
        }
        else if (WIFSTOPPED(status))
            printf("\n%s with pid %d suspended normally\n", name, pid);
        else
        {
            printf("\n%s with pid %d exited abnormally\n", name, pid);
            removeJob(pid);
        }
        prompt();
        fflush(stdout);
    }
}
