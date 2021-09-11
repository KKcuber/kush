#include "headers.h"

void init_shell()
{
    getcwd(tempHome, sizeof(tempHome));
    char* lusername = getpwuid(getuid())->pw_name;
    strcpy(username, lusername);
    gethostname(sysname, sizeof(sysname)); 
    prompt(); 
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
        strcat(prompt, ">");
    }
    else{
        strcat(prompt, ">");
        strcat(prompt, cwd);
    }
    printf("%s %s", prompt, username);
}