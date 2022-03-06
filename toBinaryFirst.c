#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CUT "\t "

int IC = 100;
char *commands[] = {"mov" , "cmp" , "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};

typedef struct commandsStruct{
    char commandName[31];
    int opcode;
    int funct;
    int numOfParam;
    /*char** sourceAble;  legit source functions */
    /*char** destAble;    legit destination functions */
     
}commandsStruct;

commandsStruct ourCommands[] = {
    /* first group -> have 2 operands*/
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

typedef struct wordsInCommand
{
    int word[20];
    wordsInCommand *next;
}wordsInCommand;

typedef struct command
{
    wordsInCommand *wordHead;
    command *next;
}command;

typedef struct symbolLink
{
	char name[31];
	int adress;
	int placing; /* 0-code image, 1-data image */
	int visibility; /* 0-internal, 1-entry, 2-extern */
	struct symbolLink *next;

}symbolLink;

symbolLink *push(symbolLink *head, char lableName[])
{
    struct symbolLink *link = (struct symbolLink *)malloc(sizeof(struct symbolLink));
    strcpy(link->name, lableName);
    link->next = head;
    head = link;
    return head;
}

struct symbolLink* find(struct symbolLink *head, char lableName[]) {
    struct symbolLink* current = head;
    if(head == NULL)
        return NULL;
   while(current != NULL) {
        if(!strcmp(current->name, lableName))
            return current;
        else 
         current = current->next;
   }
   return NULL;
}

symbolLink * symboleTableCreat(FILE *filePointer)
{
    int i = 0;
    char line[81];
    char *lableName;
    symbolLink *head;
    symbolLink *lableFound;
    while (fgets(line, 81, filePointer))
    {
        lableName = strtok(line, CUT);
        if(lableName[strlen(lableName)-1] == ':')
            head = push(head, lableName);
        else if(!strcmp(lableName, ".extern"))
        {
            lableName = strtok(NULL, CUT);
            head = push(head, lableName);
            head->visibility = 2;
        } else if(!strcmp(lableName, ".entry"))
        {
            lableFound = find(head, lableName);
            if(lableFound == NULL)
            {
                head = push(head, lableName);
                head->visibility = 1;
            }else
                lableFound->visibility = 1;
        }
    }
}

int isLable(char *lable)
{
    if(lable[strlen(lable)-1] == ':')
        return 1;
    return 0;
}

int isInstruction(char *command)
{
    int i;
    for(i = 0; i <= 16; i++)
    {
        if(!strcmp(command, commands[i]))
            return 1;
    }
    return 0;
}

int isRegister(char *param)
{
    char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
    int i;
    for(i = 0; i < 17; i++)
    {
        if(!strcmp(param, registers[i]))
            return 1;
    }
    return 0;
}

wordsInCommand *toBinary(char line[])
{
    char *command;
    int numOfOperands;
    int word[20];
    char *param1;
    char *param2;
    commandsStruct commandInLine;
    wordsInCommand *headForLine = (struct wordsInCommand*)malloc(sizeof(struct wordsInCommand));
    if(isInstruction(line))
    {
        command = strtok(line, CUT);
        int i = 0;
        for(; i <=16; i++)
        {
            if(!strcmp(command, ourCommands[i].commandName))
                commandInLine = ourCommands[i];
        }
        numOfOperands = commandInLine.numOfParam;
        headForLine->word[18] = 1;
        headForLine->word[commandInLine.opcode] = 1;
        IC++;
        if(numOfOperands == 1)
        {
            param1 = strtok(NULL, CUT);
            if(isRegister(param1))
            {
                
            }
        } 
    }
}

void firstPass(FILE *filePointer, symbolLink *headOfTable)
{
    int IC = 100;
    int DC = 0;
    char line[81];
    char lineCopy[81];
    char *token;
    symbolLink *lable;
    
    while(fgets(line, 81, filePointer))
    {
        strcpy(lineCopy, line);
        token = strtok(line, CUT);
        if(isLable(token))
        {
            lable = find(headOfTable, token);
            lable->adress = IC;
            token = strtok(NULL, '\n');
            /*a methd not written yet that maked the binary lines for each line*/
        }else if ((!strcmp(token, ".extern"))||(!strcmp(token, ".entry")))
            continue;;
        else
        /*a methd not written yet that maked the binary lines for each line*/
    }
}


