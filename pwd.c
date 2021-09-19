#include "headers.h"

// just gets the cwd and prints it
void pwd()
{
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
}