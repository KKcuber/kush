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
        char *commandsSavePointer;
        commands[0] = strtok_r(line, ";\n", &commandsSavePointer); //separating the commands
        int numCommands = 0;
        while (commands[numCommands] != NULL)
        {
            numCommands++;
            commands[numCommands] = strtok_r(NULL, ";\n", &commandsSavePointer);
        }

        // iterating over each command

        for (int j = 0; j < numCommands; j++)
        {
            // Separating the command into pipes
            char * pipeSavePointer;
            pipes[0] = strtok_r(commands[j], "|", &pipeSavePointer); //separating the pipes
            int numpipecommands = 0;
            while (pipes[numpipecommands] != NULL)
            {
                numpipecommands++;
                pipes[numpipecommands] = strtok_r(NULL, "|", &pipeSavePointer);
            }

            if(numpipecommands == 1)
            {
                // if there is only one command, then we can just execute it
                char * tokenSavePointer;
                token[0] = strtok_r(pipes[0], " \t\r\n", &tokenSavePointer); //separating the tokens
                int numTokens = 0;
                while(token[numTokens] != NULL) //Separating tokens within the command
                {
                    numTokens++;
                    token[numTokens] = strtok_r(NULL, " \t\r\n", &tokenSavePointer);
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
                int new_fds[2];
                int old_fds[2];

                // iterating over each pipe

                for (int k = 0; k < numpipecommands; k++)  
                {

                    // Separating the commands into tokens
                    char * tokenSavePointer;
                    token[0] = strtok_r(pipes[k], " \t\r\n", &tokenSavePointer);
                    int numTokens = 0;
                    while(token[numTokens] != NULL) //Separating tokens within the command
                    {
                        numTokens++;
                        token[numTokens] = strtok_r(NULL, " \t\r\n", &tokenSavePointer);
                    }
                    int numRepeat = 1;

                    if(k!=numpipecommands-1)
                    {
                        if(pipe(new_fds) < 0)
                        {
                            perror("Error: pipe failed");
                            continue;
                        }
                    }
                    // if the command is one of the in-built commands then we call the respective fucnction to execute it
                    if(strcmp(token[0], "pwd") == 0 || strcmp(token[0], "cd") == 0 || strcmp(token[0], "echo") == 0 || strcmp(token[0], "ls") == 0 || strcmp(token[0], "quit") == 0 || strcmp(token[0], "pinfo") == 0)
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
                    }
                    else
                    {
                        pipeExecute(numpipecommands, new_fds, old_fds, k);
                    }
                }
                close(old_fds[0]);
                close(old_fds[1]);
            }
        }
    } while(status);
}