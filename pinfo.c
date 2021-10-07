#include "headers.h"

// this fucnction parses the file /proc/[pid]/stat to get the required values

void pinfo(int numTokens)
{
    pid_t pid;
    if(numTokens > 2)
    {
        printf("pinfo can have only 1 argument at max\n");
        return;
    }
    char pidString[20];
    char procPath[30] = "/proc/";
    char exePath[30];
    if(numTokens == 1)
    {
        pid = getpid();
        sprintf(pidString, "%d", pid);  
    }
    else 
    {
        strcpy(pidString, token[1]);
        pid = atoi(pidString);
    }
    strcat(procPath, pidString);
    strcpy(exePath, procPath);
    strcat(exePath, "/exe");
    strcat(procPath, "/stat");
    char * line;
    long size = 0;

    // reading/proc/pid/stat to get required info
    FILE * procFile = fopen(procPath, "r");
    if(procFile == NULL)
    {
        printf("process does not exist\n");
        return;
    }
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

    printf("pid -- %s\n", pidString);
    printf("Process Status -- %s", statTokens[2]);
    if(strcmp(statTokens[4], statTokens[7]) == 0)
        printf("+");
    printf("\nmemory -- %s\n", statTokens[22]);

    // finding executable path
    char executablePath[100000] = "";
    readlink(exePath, executablePath, 100000);
    executablePath[strlen(executablePath)] = '\0';
    if(!strncmp(executablePath,tempHome, strlen(tempHome)))
    {
        char path[100000] = "~";
        strcat(path, executablePath + strlen(tempHome));
        printf("Executable Path -- %s\n", path);
    }
    else
    {
        printf("Executable Path -- %s\n", executablePath);
    }
    fclose(procFile);
}