#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h" 

void preProcessing(FILE *fileName){
    char line[MAX_LINE_LENGTH];
    MACRO *head;
    MACRO *macroFound;
    char *word;
    char *macroName;
    char *macroCommands[6];
    int inMacro = 0;
    int i, j, linesInMacro = 0;

    FILE *objectFile = fopen(strcat(fileName, ".ob"), 'w');

    while (fgets(line, MAX_LINE_LENGTH, fileName))
    {
        if(!skip(line)){
            i = getWord(word, line, move_to_none_white(line, 0));
            if(!inMacro){
                if(!strcmp(word, "macro"))
                {
                    inMacro = 1;
                    strcpy(macroName ,getWord(word, line, i));
                } else{
                    macroFound = find(head, macroName);
                    if(*macroFound != NULL){
                        for(j = 0; j < macroFound.numOfCommands; j++)
                        {
                            fputs(macroFound.macroCommands[j], objectFile);
                        }
                    }else
                        fputs(line, objectFile);
                }
            } else {
                if (!strcmp(word, "endm"))
                {
                    head = push(macroName, macroCommands, linesInMacro);
                    inMacro = 0;
                    linesInMacro = 0;
                } else{
                    strcpy(macroCommands[linesInMacro], line);
                    linesInMacro++;
                }
            }
        }
    }  
}


MACRO* find(MACRO *head, char * macroName) {
    MACRO* current = head;
    if(head == NULL)
        return NULL;

   while(strcmp(current->macroName, macroName)) {
        if(current->next == NULL)
            return NULL;
        else 
         current = current->next;
   }
   return current;
}

int getWord(char *word, char line[], int i){
    int j;
    for(j = 0; !isspace(line[i]) && line[i] != '\n'; j++){
        word[j] = line[i];
        i++;
    }
    word[j] = '\0';
    return i;
}
