#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.c"

#define LINE_LENGTH 81

void first_pass(FILE *filePointer, string fileName, int fileLength)
{
    /* receives assembly source code file name and checks for any errors,
    if none were found then it will..... @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/ 
    
    char **macroDetails;
    char line[LINE_LENGTH];
    char *word;
    int inmacro = 0, i = 0, j = 0, macroCounter = 0, linesInMacro = 1;
    
    FILE *obFile = fopen(strcat(fileName.data, ".ob"), 'w');
    macroDetails = calloc(7 * (fileLength/2) ,sizeof(char *));
    
    while (fgets(line, LINE_LENGTH, filePointer))
    {
        if(line[i] == '}')
            i++;
        
        i = move_to_none_white(line, i);
        
        for(; !isspace(line[i]); j++, i++)
                word[j] = line[i];
        
        /* check if we are not inside a macro at the moment */
        if(!inmacro)
        {
            /* finding where the macro starts */
            if (!strcmp(word, "macro"))
            {
                inmacro = 1;
                i = move_to_none_white(line, ++i);
                free(word);
                for(j = 0; !isspace(line[i]); i++, j++)
                    word[j] = line[i];
                *(macroDetails + macroCounter) = word;
            }
            
            else
                fputs(line, obFile);
        } 
        
        else 
        {
            /* finding where the macro ends */
            if (!strcmp(word, "endm"))
            {
                inmacro = 0;
                macroCounter++;
                break;
            }
            
            /* adding a command that is inside the current macro */
            else {
                *(macroDetails + macroCounter +(linesInMacro * (fileLength/2))) = line;
                linesInMacro++;
            }
        }
        free(word);
    }
    i = 0;
    while(*(macroDetails + i) != '\0')
    {
        int k = 0, l = 0;
        for(; k < fileLength; k++)
        {
            l = move_to_none_white(line, l);
            while(line[l] != '\n')
            {
                for(j = 0; !isspace(line[l]); j++, l++)
                    word[j] = line[l];
            }
        }
    }
}

