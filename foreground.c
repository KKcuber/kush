#include "headers.h"

void foreground()
{
    pid_t pid;
    pid = fork();
    curr_foregound_job.job_num = 0;
    curr_foregound_job.pid = pid;
    strcpy(curr_foregound_job.name,token[0]);

    if (pid < 0)
        perror("Error: Fork Failed\n");
    else if (pid == 0)
    {
        // setpgid(0,0);
        //signal(SIGTSTP, ctrl_z);
        int ret = execvp(token[0], token);
        if (ret < 0)
        {
            perror("Error: exec failed");
            printf("Command not found: %s\n", token[0]);
            exit(1);
        }
    }
    else
    {
        // signal(SIGTTOU, SIG_IGN);
        // signal(SIGTTIN, SIG_IGN);
        // tcsetpgrp(0, pid);
        int status=0;
        waitpid(pid,&status,WUNTRACED);
        // tcsetpgrp(0, getpgrp());
        // signal(SIGTTOU, SIG_DFL);
        // signal(SIGTTIN, SIG_DFL);
        curr_foregound_job.pid = -1;
        curr_foregound_job.name[0] = '\0';
        // wait(NULL);
    }
}