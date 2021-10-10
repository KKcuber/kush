#include "headers.h"

// function which executes the respective command
void executeCommand(int numTokens)
{
    if(strcmp(token[0], "pwd") == 0)
        pwd();
    else if(strcmp(token[0], "cd") == 0)
        cd(numTokens, token);
    else if(strcmp(token[0], "echo") == 0)
        echo(numTokens, token);
    else if(strcmp(token[0], "ls") == 0)
        ls(numTokens);
    else if (strcmp(token[0], "quit") == 0 && numTokens == 1)
        exit(0);
    else if(strcmp(token[0], "pinfo") == 0)
        pinfo(numTokens);
    else if(strcmp(token[0], "jobs") == 0)
        jobs(numTokens);
    else if(strcmp(token[0], "fg") == 0)
        fg(numTokens);
    else if(strcmp(token[0], "bg") == 0)
        bg(numTokens);
    else if(strcmp(token[0], "sig") == 0)
        sig(numTokens);
    else if(strcmp(token[numTokens-1], "&") == 0)
        background(numTokens);
    else
        foreground();
}

// checks whether a  string is a number or not
int isNumber(char* string)
{
    int i = 0;
    while(string[i] != '\0')
    {
        if(!isdigit(string[i]))
            return 0;
        i++;
    }
    return 1;
}

// command which executes a particular command in fixed time interval for a certain period of time
void replay(int numTokens)
{
    int errorflag = 0;
    int interval, period;
    for(int i = 0;i < numTokens; i++)
    {
        if(!strcmp(token[i], "-interval"))
        {
            if(i+1 < numTokens)
            {
                if(isNumber(token[i+1]))
                {
                    interval = atoi(token[i+1]);
                    i++;
                }
                else
                {
                    errorflag = 1;
                    break;
                }
            }
            else
            {
                errorflag = 1;
                break;
            }
        }
        else if(!strcmp(token[i], "-period"))
        {
            if(i+1 < numTokens)
            {
                if(isNumber(token[i+1]))
                {
                    period = atoi(token[i+1]);
                    i++;
                }
                else
                {
                    errorflag = 1;
                    break;
                }
            }
            else
            {
                errorflag = 1;
                break;
            }
        }
    }
    if (errorflag == 1)
    {
        printf("Error: Invalid arguments\n");
        return;
    }
    // finding the command to be executed
    for(int i = 0;i < numTokens; i++)
    {
        if(!strcmp(token[i], "-command"))
        {
            if(i+1 < numTokens)
            {
                int j = i+1;
                // finding last index of the token of the command to be executed
                for(; j < numTokens; j++)
                {
                    if(!strcmp(token[j], "-interval") || !strcmp(token[j], "-period"))
                        break;
                }

                // shifting token array such that it can be executed just like before
                int inc = 0;
                for(int k = i+1; k < j; k++, inc++)
                {
                    token[inc] = token[k];
                }
                token[inc] = NULL;
                numTokens = inc;
            }
            else{
                errorflag = 1;
                printf("Error: Invalid arguments\n");
                return;
            }
        }
    }
    int toAdd = interval;
    for(; interval <= period; interval = interval + toAdd)
    {
        sleep(toAdd);
        executeCommand(numTokens);
    }
    sleep(period%toAdd);
}