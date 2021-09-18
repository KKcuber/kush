#include "headers.h"

void inputLoop()
{   
    int status = 1;
    do{
        signal(SIGCHLD, childHandler);
        prompt();
        char *line;
        long size = 0;
        getline(&line, &size, stdin);

        // Separating the input into commands

        commands[0] = strtok(line, ";\n"); //separating the commands
        int numCommands = 0;
        while (commands[numCommands] != NULL)
        {
            numCommands++;
            commands[numCommands] = strtok(NULL, ";\n");
        }

        // iterating over each command

        for (int j = 0; j < numCommands; j++)
        {
            // Separating the commands into tokens

            token[0] = strtok(commands[j], " \t\r\n");
            int numTokens = 0;
            while(token[numTokens] != NULL) //Separating tokens within the command
            {
                numTokens++;
                token[numTokens] = strtok(NULL, " \t\r\n");
            }
            int numRepeat = 1;
            // Calling the respective functions according to the commands entered by the user
            if(strcmp(token[0], "repeat") == 0)
            {
                if(numTokens < 3) printf("insufficient number of arguments for repeat");
                numRepeat = atoi(token[1]);
                for(int k = 2; k < numTokens; k++)
                {
                    strcpy(token[k-2],token[k]); 
                }
                token[numTokens-2] = NULL;
                numTokens = numTokens - 2;
            }
            for(int rep = 0; rep<numRepeat; rep++)
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
                    status = 0;
                else if(strcmp(token[0], "pinfo") == 0)
                    pinfo(numTokens);
                else if(strcmp(token[numTokens-1], "&") == 0)
                    background(numTokens);
                else
                    foreground();
            }
        }
    } while(status);
}