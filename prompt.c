#include "headers.h"

void init_shell()
{
    getcwd(tempHome, sizeof(tempHome));
    getcwd(prevDir, sizeof(prevDir));
    char* lusername = getpwuid(getuid())->pw_name;
    strcpy(username, lusername);
    gethostname(sysname, sizeof(sysname)); 
    bjob_count = 0;
}

void prompt()
{
    getcwd(cwd, sizeof(cwd));
    char prompt[100000] = "<";
    strcat(prompt, username);
    strcat(prompt, "@");
    strcat(prompt, sysname);
    strcat(prompt, ":");
    if(!strncmp(cwd,tempHome, strlen(tempHome)))
    {
        strcat(prompt, "~");
        strcat(prompt, cwd + strlen(tempHome));
        strcat(prompt, "> ");
    }
    else{
        strcat(prompt, cwd);
        strcat(prompt, "> ");
    }
    printf(CYAN "%s" RESET, prompt);
}