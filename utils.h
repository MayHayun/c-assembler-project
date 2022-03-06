#include <stdio.h>
#include <stdlib.h>

#define COMMAND_MAX_LENGTH 31
#define MAX_LINE_LENGTH 81
#define MAX_COMMANDS_IN_MACRO 6

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

int isNameOk(char line []);
int isARegister(char []);
int isACommand(char []);
int isANaturalNum(char);

