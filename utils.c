#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"



int move_to_none_white(char *line, int i)
{
    for(; i < strlen(line); i++)
    {
        if(!isspace(line[i]))
            return i;
    }
    return i;
}
