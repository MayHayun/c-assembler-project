#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "firstPass.h"
#include "symbolTable.h"


int * decToBinary(int num)
{
    int *array, i;
    array = calloc(16, sizeof(int));
    for(i = 0 ; i < 17 ; i ++)    
    {    
        array[i] = num%2;    
        num = num/2;    
    }  
    return array;
}

void addLine(LINE *head, LINE *link)
{
    while(head->next != NULL)
        head = head->next;
    head->next = link;
}

void addWord(WORD *head, WORD *link)
{
    while(head->next != NULL)
        head = head->next;
    head->next = link;
}

WORD* deliveryForBinary(commandsStruct *command ,char myStr[], symbolLink *headOfTable){ 
    char tempLine[30]; 
    char tempLabel[30]; 
    char tempRegister[30]; 
    int isDest = 1;
    int i, j;
    char* token; 
    WORD* link; 
    
    token = strtok(myStr, ", \t\n");
    link->word[18] = 1;
    if(command->funct != 0)
    {
        int *functInBin;
        functInBin = decToBinary(command->funct);
        for(i = 12, j = 0; i < 16; i++, j++)
            link->word[i] = functInBin[j];
    }

    if(command->numOfParam == 0)
        return link;

    while(token != NULL)
    {
        /* Delivery 0 */ 
        if( myStr[0] == '#' && isAIntNum(token + 1))
        {
            token = strtok(NULL, ", \t\n");
            continue;
        }

        /* Delivery 1 */ 
        else if(find(headOfTable, myStr) != NULL)
            if(isDest)
                link->word[0] = 1;
            else
                link->word[6] = 1;

        /* Delivery 3 */
        else if(isARegister(token) != -1)
        {   
            int *regInBinary;
            regInBinary = decToBinary(isARegister(token));
            if(isDest)
            {
                for(i = 2, j = 0; i < 6; i++, j++)
                    link->word[i] = regInBinary + j;
                link->word[0] = 1;
                link->word[1] = 1;
            }else
            {
                for(i = 8, j = 0; i < 12; i++, j++)
                    link->word[i] = regInBinary + j;
                link->word[6] = 1;
                link->word[7] = 1;
            }
        }

        /*Delivery 2*/
        else
        {
            char *reg;
            int *regInBin;
            reg = extractRegister(token);
            regInBin = decToBinary(reg);
            if(isDest)
            {
                for(i = 2, j = 0; i < 6; i++, j++)
                    link->word[i] = regInBin + j;
                link->word[1] = 1;
            }else
            {
                for(i = 8, j = 0; i < 12; i++, j++)
                    link->word[i] = regInBin + j;
                link->word[7] = 1;
            }
        }
        isDest = 0;
        token = strtok(NULL, ",\n");
    }
    IC++;
    return link;
} 

int isNum(char *str)
{
    if(*str == '#')
        return 1;
    return 0;
}

int extractRegister(char * param)
{
    char *token;
    token = strtok(param, "[");
    token = strtok(NULL, "]");
    return isARegister(token);
}    

WORD *extraWordsToBinary(char *param)
{
    WORD *link;
    if(isNum(param))
    {
        int *numInBinary, i , num, k = 1;
        for(i = strlen(param)-1; i >= 0; i--)
        {
                if(*(param + i) == '-')
                    num = num * (-1);
                else{
                    num = num + (*(param + i) * k);
                    k = k * 10;
                }
        }
        numInBinary = decToBinary(num);
        for(i = 0; i < 16; i++)
            link->word[i] = *(numInBinary + i);
        link->word[18] = 1;
        IC++;
        return link;
    }else 
        IC = IC + 2;
    return NULL;
}

LINE *toBinaryCommand(char line[], symbolLink *headOfTable)
{
    char *command;
    char *token;
    WORD *headForLine = (struct WORD*)malloc(sizeof(struct WORD));
    LINE *node;
    commandsStruct *commandFound;
    char *restOfString;

    command = strtok(line, CUT);
    commandFound = findCommand(command);
    headForLine->word[commandFound->opcode] = 1;
    headForLine->word[18] = 1;
    IC++;
    node->wordHead = headForLine;
    restOfString = strtok(NULL, '\n');

    addWord(headForLine, deliveryForBinary(commandFound, restOfString, headOfTable));
    
    token = strtok(restOfString, ",\n");

    while(token != NULL)
    {
        if(isARegister(token) == -1)
            break;
        addWord(headForLine, extraWordsToBinary(token));
        token = strtok(NULL, ",\n");
    }
    return node;
}


int isARegister(char line []){
  if(!strcmp(line,"r0"))
    return 0;  
  if(!strcmp(line,"r1"))
    return 1;
  if(!strcmp(line,"r2"))
    return 2;  
  if(!strcmp(line,"r3"))
    return 3;
  if(!strcmp(line,"r4"))
    return 4;  
  if(!strcmp(line,"r5"))
    return 5;
  if(!strcmp(line,"r6"))
    return 6;  
  if(!strcmp(line,"r7"))
    return 7;
  if(!strcmp(line,"r8"))
    return 8;  
  if(!strcmp(line,"r9"))
    return 9;
  if(!strcmp(line,"r10"))
    return 10;  
  if(!strcmp(line,"r11"))
    return 11;
  if(!strcmp(line,"r12"))
    return 12;  
  if(!strcmp(line,"r13"))
    return 13;
  if(!strcmp(line,"r14"))
    return 14;  
  if(!strcmp(line,"r15"))
    return 15;
  return -1;
}


void firstPass(FILE *filePointer)
{
    int DC = 0;
    char line[81];
    char lineCopy[81];
    char *token;
    char *firstWord;
    symbolLink *lable;
    symbolLink *headOfTable = symboleTableCreat(filePointer);
    LINE *headOfFile;
    headOfFile = NULL;
    
    while(fgets(line, 81, filePointer))
    {
        strcpy(lineCopy, line);
        token = strtok(line, ":\t ");
        if((lable = findSymbol(headOfTable, token)) != NULL){
            char *tokenCopy;
            lable->adress = IC;
            token = strtok(NULL, '\n');
            strcpy(tokenCopy, token);
            firstWord = strtok(token, CUT);
            if(isACommand(firstWord))
                if(IC == 100)
                    headOfFile = toBinaryCommand(tokenCopy, headOfTable);
                else
                    addLine(headOfFile, toBinaryCommand(token, headOfTable));
            else
                /*toBinaryGuidance*/
        }else if(!strcmp(token, ".extern")){
            token = strtok(NULL, CUT);
            lable = findSymbol(headOfTable, token);
            lable->visibility = 2;
        }else if(!strcmp(token, ".entry")){
            token = strtok(NULL, CUT);
            lable = findSymbol(headOfTable, token);
            lable->visibility = 1;
        } else if(isACommand(token))
            if(IC == 100)
                headOfFile = toBinaryCommand(lineCopy, headOfTable);
            else
                addLine(headOfFile, toBinaryCommand(lineCopy, headOfTable));
        else
            /*toBinaryGuidance*/
        
    }
}
