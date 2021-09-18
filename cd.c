#include "headers.h"

void cd (int numTokens, char** token)
{
	char tempPrevDir[100000];
    getcwd(tempPrevDir, sizeof(tempPrevDir));
    if(numTokens > 2)
    {
        printf("cd command can have only one argument\n");
        return;
    }
	if (numTokens == 1 || strcmp(token[1], "~") == 0){
		chdir(tempHome);
	}
	if(strcmp(token[1], "-") == 0)
		chdir(prevDir);
	else{
		int err = chdir(token[1]);
		if (err < 0){
			perror("chdir failed");
		}
	}
	strcpy(prevDir, tempPrevDir);
}