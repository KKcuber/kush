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
	else if (numTokens == 1 || strcmp(token[1], "~") == 0){
		int err = chdir(tempHome);
		if (err < 0){
			perror("chdir failed");
		}
	}
	else if(strcmp(token[1], "-") == 0)
		chdir(prevDir);
	else{
		int err = chdir(token[1]);
		if (err < 0){
			perror("chdir failed");
		}
	}
	strcpy(prevDir, tempPrevDir);
}