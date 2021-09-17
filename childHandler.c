#include "headers.h"

void childHandler()
{
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED )) > 0)
    {
        char* name = "kush";// get_job_name(pid);
        if (name == NULL)
        {
            continue;
        }

        if (WIFEXITED(status)) {
            printf("\n%s with pid %d exited normally\n",name, pid);
            // remove_job(pid);
        }else if (WIFSTOPPED(status)) {
            printf("%s with pid %d suspended normally\n",name, pid);
            // remove_job(pid);
        }else {
            printf("%s with pid %d did not exit normally\n",name, pid);
            // remove_job(pid);
        }
        prompt();
        fflush(stdout);
    }
}
