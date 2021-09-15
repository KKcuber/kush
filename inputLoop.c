#include "headers.h"

void inputLoop()
{   
    int status = 1;
    do{
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

            // Calling the respective functions according to the commands entered by the user

            if(strcmp(token[0], "pwd") == 0)
                pwd();
            else if(strcmp(token[0], "cd") == 0)
                cd(numTokens, token);
            else if(strcmp(token[0], "echo") == 0)
                echo(numTokens, token);
            else if(strcmp(token[0], "ls") == 0)
                ls(numTokens);
            else if (strcmp(token[0], "quit") == 0)
                status = 0;
        }
    } while(status);
}