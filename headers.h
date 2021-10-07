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
#include <stdlib.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

char cwd[1000];
char tempHome[1000];
char sysname[1000];
char username[1000];
char *commands[1000];
char *pipes[1000];
char *token[1000];
char prevDir[1000];
pid_t shellpid;


struct jobs{
    char name[100];
    pid_t pid;
    int job_num;
};
typedef struct jobs bjob;

int bjob_count;
bjob job_arr[100];
int job_num_available[100];
bjob curr_foregound_job;

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
void pinfo(int numTokens);
void pipeExecute(int numTokens, int new_fd[], int old_fd[], int k);
void ctrl_c();
void ctrl_z();
void jobs(int numTokens);
void sig(int numTokens);
void bg(int numTokens);
void fg(int numTokens);
int getJobIndex_givenPid(int pid);