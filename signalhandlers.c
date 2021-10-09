#include "headers.h"

void ctrl_c()
{
    pid_t curpid = getpid();
    if(curr_foregound_job.pid > 0)
    {
        kill(curr_foregound_job.pid, SIGINT);
        curr_foregound_job.pid = -1;
        curr_foregound_job.name[0] = '\0';
    }
    else if(curpid == shellpid)
    {
        printf("\n");
        prompt();
        fflush(stdout);
    }
}

void ctrl_z()
{
    pid_t curpid = getpid();
    if(curr_foregound_job.pid > 0)
    {
        kill(curr_foregound_job.pid, SIGTSTP);
        
        //checking if the job already exists in the job_arr or not, and then adding it if it doesn't
        int exists = 0;
        for(int ijob = 0; ijob < 100; ijob++)
        {
            if(curr_foregound_job.pid == job_arr[ijob].pid)
            {
                exists = 1;
                break;
            }
        }
        if(!exists)
        {
            job_arr[bjob_count].pid = curr_foregound_job.pid;
            strcpy(job_arr[bjob_count].name, curr_foregound_job.name);
            for(int ijob = 1; ijob < 100; ijob++)
            {
                if(job_num_available[ijob] == 0)
                {
                    job_num_available[ijob] = 1;
                    job_arr[bjob_count].job_num = ijob;
                    break;
                }
            }
            bjob_count++;
            curr_foregound_job.pid = -1;
            curr_foregound_job.name[0] = '\0';
        }
        else{
            curr_foregound_job.pid = -1;
            curr_foregound_job.name[0] = '\0';
        }
    }
    else //if(curpid == shellpid)
    {
        printf("\n");
        prompt();
        fflush(stdout);
    }
}