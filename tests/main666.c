#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 81



typedef struct MACRO
{
    char * macroName;
    char * macroCommands[6];
    int numOfCommands;
    struct MACRO *next; 
}MACRO;


struct MACRO *head = NULL;
int move_to_none_white(char line[], int i);
int skip(char line[]);
void preProcessing(FILE *fileName, char *nameOfFile);
MACRO* find(MACRO *head, char * macroName);
int getWord(char *word, char line[], int i);
MACRO *push(char *macroName, char *macroCommands[], int numOfCommands);



 int move_to_none_white(char *line, int i)
{
    for(; i < strlen(line); i++)
    {
        if(!isspace(line[i]))
            return i;
    }
    return i;
}
 
 int skip(char line[])
 {
     int i = 0;
     i = move_to_none_white(line, i);
     if(line[i] == ':' || line[i] == '\n')
         return 1;
     return 0;
 }

 MACRO *push(char *macroName, char *macroCommands[], int numOfCommands){
     int i = 0;
     struct MACRO *link = (struct MACRO*) malloc(sizeof(struct MACRO));
     strcpy(link->macroName, macroName);
     for(; i < 6; i++){
         strcpy(link->macroCommands[i], macroCommands[i]);
     }
     link->numOfCommands = numOfCommands;
     link->next = head;
     head = link;
     return head;
 }
 


void preProcessing(FILE *fileName, char *nameOfFile){
    char line[MAX_LINE_LENGTH];
    MACRO *head;
    MACRO *macroFound;
    char *word = NULL;
    char *macroName =NULL;
    char *macroCommands[6];
    int inMacro = 0;
    int i, j, linesInMacro = 0;

    FILE *objectFile = fopen(strcat(nameOfFile, ".ob"), "w");
    printf("\n hello\n");
    while (fgets(line, MAX_LINE_LENGTH, fileName))
    {
        printf("\n hello666\n");
        if(!skip(line)){
            i = getWord(word, line, move_to_none_white(line, 0));
            if(!inMacro){
                if(!strcmp(word, "macro"))
                {
                    inMacro = 1;
                    getWord(word, line, i);
                    strcpy(macroName ,word);
                } else{
                    macroFound = find(head, macroName);
                    if(macroFound != NULL){
                        for(j = 0; j < macroFound->numOfCommands; j++)
                        {
                            fputs(macroFound->macroCommands[j], objectFile);
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

int main()
{
    FILE *fp;
    char fileName[10];
    char temp[10];
    char temp2[10];
    int i;
    fgets(fileName, 10, stdin);
    printf("\n\n welp\n\n");
    for(i = 0; i < strlen(fileName) - 1; i++)
    {
        temp[i] = fileName[i];
    }
    temp[i] = '\0';
    strcpy(temp2, temp);
    strcat(temp, ".as");
    printf("%s", temp);
    fp = fopen(temp, "r");
    printf("\n\n welp6666666\n\n");
    preProcessing(fp, temp2);
    printf("an object file was created");
    return 0;
}
