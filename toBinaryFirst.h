#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "symbolTable.h"


int IC = 100;
int DC = 0;

typedef struct commandsStruct{
    char commandName[31];
    int opcode;
    int funct;
    int numOfParam;     
}commandsStruct;

commandsStruct ourCommands[] = {
    {"mov", 0, 0, 2},
    {"cmp", 1, 0, 2},
    {"add", 2, 10, 2},
    {"sub", 2, 11, 2},
    {"lea", 4, 0, 2},

    /* second group -> have 1 operand */
    {"clr", 5, 10, 1},
    {"not", 5, 11, 1},
    {"inc", 5, 12, 1},
    {"dec", 5, 13, 1},
    {"jmp", 9, 10, 1},
    {"bne", 9, 11, 1},
    {"jsr", 9, 12, 1},
    {"red", 12, 0, 1},
    {"prn", 13, 0, 1},

    /* third group -> no operands */
    {"rts", 14, 0, 0},
    {"stop", 15, 0, 0}
};

typedef struct WORD
{
    int word[20];
    WORD *next;
}WORD;

typedef struct LINE
{
    WORD *wordHead;
    LINE *next;
}LINE;

char *cutWhiteChars(char *str);

LINE *firstPass(FILE *filePointer, symbolLink *headOfTable);
LINE *toBinaryCommand(char line[], symbolLink *headOfTable);
LINE *toBinaryGuidance(char line[]);
WORD *deliveryForBinary(commandsStruct *command ,char myStr[], symbolLink *headOfTable);
WORD *extraWordsToBinary(char *param);
WORD *charToBinary(char ch);
void addLine(LINE *head, LINE *link);
void addWord(WORD *head, WORD *link);
int isNum(char *str);
int extractRegister(char * param);
int isARegister(char line []);
int * decToBinary(int num);
