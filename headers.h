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

char cwd[100000];
char tempHome[100000];
char sysname[100000];
char username[1000000];
char *commands[100000];
char *token[100000];

void init_shell();
void prompt();
void inputLoop();
void pwd();
void echo(int numTokens, char **token);
void cd (int numTokens, char** token);
void ls(int numTokens);
