#include "headers.h"

int strcmp(const char *X, const char *Y)
{
    while (*X)
    {
        // if characters differ, or end of the second string is reached
        if (*X != *Y) {
            break;
        }
 
        // move to the next pair of characters
        X++;
        Y++;
    }
 
    // return the ASCII difference after converting `char*` to `unsigned char*`
    return *(const unsigned char*)X - *(const unsigned char*)Y;
}

void inputLoop()
{   
    int status = 1;
    do{
        prompt();
        char *line;
        long size = 0;
        getline(&line, &size, stdin);

        // Separating the input into commands

        commands[0] = strtok(line, ";\n"); 
        long long numCommands = 0;
        for(;commands[numCommands] != NULL; numCommands++)
        {
            commands[numCommands] = strtok(NULL, ";\n");
        }

        // iterating over each command

        for (long long j = 0; j < numCommands; j++)
        {
            // Separating the commands into tokens

            token[0] = strtok(commands[j], " \t\n");
            long long numTokens = 0;
            for(;token[numTokens] != NULL; numTokens++)
            {
                token[numTokens] = strtok(NULL, ";\n");
            }

            // Calling the respective functions according to the commands entered by the user

            if(strcmp(token[0], "pwd") == 0)
                pwd();
            // else if(strcmp(token[0], "cd") == 0)
            //     cd(token, numTokens);
            // else if(strcmp(token[0], "echo") == 0)
            //     echo(token, numTokens);
            // else if(strcmp(token[0], "ls") == 0)
            //     ls(cwd, numTokens, token);
            else if (strcmp(token[0], "quit") == 0)
                status = 0;
        }
    } while(status);
}