#include "headers.h"

void background(int numTokens)
{
    pid_t pid = fork(); 
    if (pid < 0)
        perror("Fork Failed");
    else if (pid == 0)
    {
        setpgid(0, 0);
        token[numTokens-1] =  NULL;
        int ret = execvp(token[0], token);
        if (ret < 0)
            perror("execvp failed");
    }
    else
    {
        setpgid(pid, pid);
        printf("%d\n", pid);
        job_arr[bjob_count].pid = pid;
        strcpy(job_arr[bjob_count].name, token[0]);
        bjob_count++;
    }
}