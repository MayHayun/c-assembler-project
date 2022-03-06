#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define CUT "\t\n "
#define MAX_LINE_LENGTH 81
#define MAX_MACRO_COMMANDS 6


typedef struct MACRO
{
    char macroName[MAX_LINE_LENGTH];
    char macroCommands[MAX_MACRO_COMMANDS][MAX_LINE_LENGTH];
    int numOfCommands;
    struct MACRO *next; 
}MACRO;


struct MACRO *head = NULL;
int move_to_none_white(char line[], int i);
int skip(char line[]);
void preProcessing(FILE *fileName, char *nameOfFile);
MACRO* find(MACRO *head, char * macroName);
/*char * getWord(char *word, char line[], int i);*/
MACRO *push(char macroName[], char macroCommands[][MAX_LINE_LENGTH], int numOfCommands, MACRO *head);



 int move_to_none_white(char line[], int i)
{
    for(; i < MAX_LINE_LENGTH-1; i++)
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
     if(line[i] == ';' || line[i-1] == '\n')
         return 1;
     return 0;
 }

 MACRO *push(char macroName[], char macroCommands[][MAX_LINE_LENGTH], int numOfCommands, MACRO *head){
     int i = 0;
     MACRO *link = (struct MACRO*)malloc(sizeof(struct MACRO));
     strcpy(link->macroName, macroName);
     for(; i < numOfCommands; i++){
         strcpy(link->macroCommands[i], macroCommands[i]);
     }
     link->numOfCommands = numOfCommands;
     link->next = head;
     head = link;
     return head;
 }

void preProcessing(FILE *fileName, char *nameOfFile){
    char line[MAX_LINE_LENGTH];
    char lineCopy[MAX_LINE_LENGTH];
    char *token;
    MACRO *head = NULL;
    MACRO *macroFound;
    char macroName[MAX_LINE_LENGTH];
    char macroCommands[MAX_MACRO_COMMANDS][MAX_LINE_LENGTH];
    int inMacro = 0;
    int j, linesInMacro = 0;

    FILE *objectFile = fopen(strcat(nameOfFile, ".ob"), "w");
    while (fgets(line, MAX_LINE_LENGTH, fileName))
    {
        strcpy(lineCopy,line);
        if(!skip(line)){
            token = strtok(lineCopy, CUT);
            if(inMacro == 0){
                if(!strcmp(token, "macro")){
                    inMacro = 1;
                    token = strtok(NULL, CUT);
                    strcpy(macroName ,token);
                } else{
                    macroFound = find(head, token);
                    if(macroFound != NULL){
                        for(j = 0; j < macroFound->numOfCommands; j++)
                            fputs(macroFound->macroCommands[j], objectFile);
                    }else
                        fputs(line, objectFile);
                }
            } else {
                if (!strcmp(token, "endm"))
                {
                    head = push(macroName, macroCommands, linesInMacro, head);
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


struct MACRO* find(struct MACRO *head, char macroName[]) {
    struct MACRO* current = head;
    if(head == NULL)
        return NULL;
   while(current != NULL) {
        if(!strcmp(current->macroName, macroName))
            return current;
        else 
         current = current->next;
   }
   return NULL;
}

int main()
{
    FILE *fp;
    char fileName[10];
    char temp[10];
    char temp2[10];
    int i;
    fgets(fileName, 10, stdin);
    for(i = 0; i < strlen(fileName) - 1; i++)
    {
        temp[i] = fileName[i];
    }
    temp[i] = '\0';
    strcpy(temp2, temp);
    strcat(temp, ".as");
    fp = fopen(temp, "r");
    preProcessing(fp, temp2);
    printf("an object file was created");
    return 0;
}
