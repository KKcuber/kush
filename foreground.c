#include "headers.h"

void foreground()
{
    pid_t pid;
    pid = fork();

    if (pid < 0)
        perror("Error: Fork Failed\n");
    else if (pid == 0)
    {
        setpgid(0,0);
        int ret = execvp(token[0], token);
        if (ret < 0)
        {
            perror("Error: exec failed\n");
            printf("Command not found: %s\n", token[0]);
        }
    }
    else
    {
        signal(SIGTTOU, SIG_IGN);
        // signal(SIGTTIN, SIG_IGN);
        tcsetpgrp(0, pid);
        int status=0;
        waitpid(pid,&status,WUNTRACED);
        tcsetpgrp(0, getpgrp());
        signal(SIGTTOU, SIG_DFL);
        // signal(SIGTTIN, SIG_DFL);
        // wait(NULL);
    }
}