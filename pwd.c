#include "headers.h"

void pwd()
{
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
}