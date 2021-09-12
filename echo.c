#include "headers.h"

void echo(int numTokens, char **token)
{
    char output[2000] = "";
    for(int i = 1; i < numTokens; i++){
        if(i>1){
            strcat(output, " ");
        }
        strcat(output, token[i]);
    }
    printf("%s\n", output);
}