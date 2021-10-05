#include "headers.h"

void inputLoop()
{   
    int stdout_fd = dup(1);
    int stdin_fd = dup(0);
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
            // Separating the command into pipes

            pipes[0] = strtok(line, "|"); //separating the pipes
            int numpipecommands = 0;
            while (pipes[numpipecommands] != NULL)
            {
                numpipecommands++;
                pipes[numpipecommands] = strtok(NULL, ";\n");
            }

            if(numpipecommands == 1)
            {
                // if there is only one command, then we can just execute it

                token[0] = strtok(pipes[j], " \t\r\n");
                int numTokens = 0;
                while(token[numTokens] != NULL) //Separating tokens within the command
                {
                    numTokens++;
                    token[numTokens] = strtok(NULL, " \t\r\n");
                }
                int tempNumTokens = numTokens;
                for(int t = 0;t < numTokens; t++)
                {
                    // checking for redirection

                    if(strcmp(token[t], ">") == 0)
                    {
                        token[t] = NULL;
                        int fdout = open(token[t+1], O_WRONLY | O_CREAT, 0644);
                        dup2(fdout, 1);
                        close(fdout);
                        tempNumTokens = t;
                    }
                    else if(strcmp(token[t], "<") == 0)
                    {
                        token[t] = NULL;
                        int fdin = open(token[t+1], O_RDONLY);
                        if(fdin < 0)
                        {
                            perror("Error: File not found");
                            continue;
                        }
                        dup2(fdin, 0);
                        close(fdin);
                        tempNumTokens = tempNumTokens<t?tempNumTokens:t;
                    }
                    else if(strcmp(token[t], ">>") == 0)
                    {
                        token[t] = NULL;
                        int fdout = open(token[t+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                        dup2(fdout, 1);
                        close(fdout);
                        tempNumTokens = tempNumTokens<t?tempNumTokens:t;
                    }
                }
                numTokens = tempNumTokens;
                int numRepeat = 1;

                // Calling the respective functions according to the commands entered by the user
                if(strcmp(token[0], "repeat") == 0)
                {
                    if(numTokens < 3) printf("insufficient number of arguments for repeat");
                    numRepeat = atoi(token[1]);
                    for(int k = 2; k < numTokens; k++)
                    {
                        //strcpy(token[k-2],token[k]); 
                        token[k-2] = token[k];
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
                dup2(stdout_fd, 1);
                dup2(stdin_fd, 0);
            }

            else
            {
                int fddes[2];
                // iterating over each pipe

                for (int k = 0; k < numpipecommands; k++)  
                {
                    if(k != 0)
                    {
                        if(pipe(fddes) == -1)
                        {
                            perror("pipe");
                            exit(EXIT_FAILURE);
                        }
                        dup2(fddes[0], STDIN_FILENO);
                    }

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
                            //strcpy(token[k-2],token[k]); 
                            token[k-2] = token[k];
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
            }
        }
    } while(status);
}