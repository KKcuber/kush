#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <grp.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <langinfo.h>
#include <signal.h>

char cwd[100000];
char tempHome[100000];
char sysname[100000];
char username[1000000];
char *commands[100000];
char *token[100000];
int curid;

struct jobs{
    char name[10000];
    pid_t pid;
};
typedef struct jobs bjob;

int bjob_count;
bjob job_arr[100000];
bjob current_fore;

void init_shell();
void prompt();
void inputLoop();
void pwd();
void echo(int numTokens, char **token);
void cd (int numTokens, char** token);
void ls(int numTokens);
void foreground();
void background(int numTokens);
void childHandler();