#include "headers.h"

// This command prints a list of all currently running background processes spawned by the shell in alphabetical order of the command name, along with their job number
void jobs(int numTokens)
{
    if(numTokens > 2)
    {
        printf("jobs: too many arguments\n");
        return;
    }
    struct currentJobs{
        char name[100];
        pid_t pid;
        int job_num;
        int isRunning;
    };
    struct currentJobs jobs[20];
    int currJobCount = 0;
    for(int i = 0; i < bjob_count; i++)
    {
        if(job_arr[i].pid > 0)
        {
            strcpy(jobs[currJobCount].name, job_arr[i].name);
            jobs[currJobCount].pid = job_arr[i].pid;
            jobs[currJobCount].job_num = job_arr[i].job_num;

            //opening the /proc/pid/stat file to check if the process is running
            char procPath[30] = "/proc/";
            char pidString[20];
            sprintf(pidString, "%d", jobs[currJobCount].pid);
            strcat(procPath, pidString);
            strcat(procPath, "/stat");
            FILE * procFile = fopen(procPath, "r");
            if(procFile == NULL)
            {
                printf("process does not exist\n");
                return;
            }
            char * line;
            long size = 0; 
            getline(&line, &size, procFile);

            //Separating tokens within stat
            char* statTokens[60];
            char* statTokensSavePtr;
            statTokens[0] = strtok_r(line, " \n", &statTokensSavePtr);
            int index = 0;
            while(statTokens[index] != NULL)
            {
                index++;
                statTokens[index] = strtok_r(NULL, " \n", &statTokensSavePtr);
            }

            //Checking if process is running and setting the isRunning flag accordingly
            if(strcmp(statTokens[2], "R") == 0 || strcmp(statTokens[2], "S") == 0)
            {
                jobs[currJobCount].isRunning = 1;
            }
            else
            {
                jobs[currJobCount].isRunning = 0;
            }
            currJobCount++;
        }
    }

    //Sorting the jobs array by name
    for(int i = 0; i < currJobCount; i++)
    {
        for(int j = i + 1; j < currJobCount; j++)
        {
            if(strcmp(jobs[i].name, jobs[j].name) > 0)
            {
                struct currentJobs temp = jobs[i];
                jobs[i] = jobs[j];
                jobs[j] = temp;
            }
        }
    }
    
    //Printing the jobs according to flag
    if(numTokens == 1)
    {
        for(int i = 0; i < currJobCount; i++)
        {
            {
                printf("[%d] %s %s [%d]\n", jobs[i].job_num,(jobs[i].isRunning==1) ? "Running" : "Stopped", jobs[i].name, jobs[i].pid);
            }
        }
    }
    // printing the jobs which are running if flag is -r
    else if(strcmp(token[1], "-r") == 0)
    {
        for(int i = 0; i < currJobCount; i++)
        {
            if(jobs[i].isRunning == 1)
            {
                printf("[%d] %s %s [%d]\n", jobs[i].job_num,(jobs[i].isRunning==1) ? "Running" : "Stopped", jobs[i].name, jobs[i].pid);
            }
        }
    }
    // printing the jobs which are stopped if flag is -s
    else if(strcmp(token[1], "-s") == 0)
    {
        for(int i = 0; i < currJobCount; i++)
        {
            if(jobs[i].isRunning == 0)
            {
                printf("[%d] %s %s [%d]\n", jobs[i].job_num,(jobs[i].isRunning==1) ? "Running" : "Stopped", jobs[i].name, jobs[i].pid);
            }
        }
    }
    else
    {
        printf("jobs: invalid flag\n");
    }
}