#include "headers.h"

// this function executes a command in a pipeline
void pipeExecute(int numPipeCommands, int new_fds[], int old_fds[], int k)
{
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork failed");
    }
    else if(pid == 0)
    {
        setpgid(0, 0);
        if(k != 0)
        {
            dup2(old_fds[0], 0);
            close(old_fds[0]);
            close(old_fds[1]);
        }
        if(k != numPipeCommands - 1)
        {
            dup2(new_fds[1], 1);
            close(new_fds[0]);
            close(new_fds[1]);
        }
        if(execvp(token[0], token) < 0)
        {
            perror("execvp failed");
            printf("Command not found: %s\n", token[0]);
            exit(1);
        }
    }
    else{
        if(k != 0)
        {
            close(old_fds[0]);
            close(old_fds[1]);
        }
        if(k != numPipeCommands - 1)
        {
            old_fds[0] = new_fds[0];
            old_fds[1] = new_fds[1];
        }
        // wait(NULL);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        tcsetpgrp(0, pid);
        int status=0;
        waitpid(pid,&status,WUNTRACED);
        tcsetpgrp(0, getpgrp());
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
    }
}