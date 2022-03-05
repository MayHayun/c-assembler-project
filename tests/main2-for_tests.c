#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

int main()
{
    FILE *fp;
    char fileName[10];
    fgets(fileName, 10, stdin);
    fp = fopen(strcat(fileName, ".as"), "r");
    preProcessing(fp, fileName);
    printf("an object file was created");
    return 0;
}