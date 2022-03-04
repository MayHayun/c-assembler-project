#include <stdio.h>
#include <stdlib.h>

#define COMMAND_MAX_LENGTH 31
#define MAX_LINE_LENGTH 81


typedef struct string{
    char *data;
}string;

typedef struct commandsStruct{
    char commandName[COMMAND_MAX_LENGTH];
    int opcode;
    int funct;
    int numOfParam;
    /*char** sourceAble;  legit source functions */
    /*char** destAble;    legit destination functions */
     
}commandsStruct;

typedef struct commandData{
    char name[10];
    int valid;
    int bytes;
}commandData;


int move_to_none_white(char line[], int i);
int skip(char line[]);
MACRO *push(char *macroName, char *macroCommands[], int numOfCommands);


typedef struct MACRO
{
    char * macroName;
    char * macroCommands[6];
    int numOfCommands;
    struct MACRO *next; 
}MACRO;


struct MACRO *head = NULL;
