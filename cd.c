#include "headers.h"

void cd (int numTokens, char** token)
{
    if(numTokens > 2)
    {
        printf("cd command can have only one argument\n");
        return;
    }
	if (numTokens == 1 || strcmp(token[1], "~") == 0){
		chdir(tempHome);
	}
	else{
		int err = chdir(token[1]);
		if (err < 0){
			perror("chdir failed");
		}
	}
}