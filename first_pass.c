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
        if(!ignore(line))
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
                    strcpy(word, '\0');
                    for(j = 0; !isspace(line[i]); i++, j++)
                        word[j] = line[i];
                    strcpy(*(macroDetails + macroCounter), word);
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
                }
            
                /* adding a command that is inside the current macro */
                else {
                    strcpy(*(macroDetails + macroCounter +(linesInMacro * (fileLength/2))), line);
                    linesInMacro++;
                }
            }
            strcpy(word, '\0');
        }
    }
}


int ignore(char line[])
{
    int i = 0;
    i = move_to_none_white(line, i);
    if(line[i] == ':' || line[i] == '\n')
        return 1;
    return 0;
}

int macro(char ** macro_details, char * word)
{
    int i = 0;
}
