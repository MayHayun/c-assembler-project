#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "first_pass.h"

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

LINE *toBinaryCommand(char line[], symbolLink *headOfTable)
{
    char *command;
    int numOfParam, opcode, funct;
    WORD *headForLine = (struct WORD*)malloc(sizeof(struct WORD));
    LINE *node;
    commandsStruct *commandFound;
    node->wordHead = headForLine;
    command = strtok(line, CUT);
    commandFound = findCommand(command);
    headForLine->word[commandFound->opcode] = 1;
    headForLine->word[18] = 1;
    IC++;
    if(commandFound->numOfParam == 0)
        return node;
    else if(commandFound->numOfParam == 1)
    {
        char *param = strtok(NULL, '\n');
        WORD *link;
        if(isARegister(param) != -1)
        {
            int regNum = isARegister(param);
            int *regInBinary;
            int *num;
            int i = 2, j;
            regInBinary = decToBinary(regNum);
            for(j = 0; i < 6; i++, j++)
                link->word[i] = regInBinary[j];
            link->word[0] = 1;
            link->word[1] = 1;
            if(commandFound->funct != 0){
                int *functInBinary;
                functInBinary = decToBinary(commandFound->funct);
                for(i = 12, j = 0; i < 16; i++, j++)
                    link->word[i] = functInBinary[j];
                link->word[18] = 1;
            }
            addWord(headForLine, link);
            IC++;
            return node;
        }else if(isANum(param)){
            int *num;
            int i, j ,k = 1;
            int *numInBinary;
            num = param+1;
            int n;
            WORD *link2;
            for(j = 0,i = strlen(num)-1; i >= 0; i++, j++){
                if(num[i] == '-')
                    n = n * (-1);
                else{
                    n = n + (num[i]*k);
                    k = k * 10;
                }
            }
            numInBinary = decToBinary(n);
            link->word[18] = 1;
            addWord(headForLine , link);
            for(i = 0; i < 17; i++)
                link2->word[i] = numInBinary[i];
            addWord(headForLine, link2);
            IC = IC + 2;
            return node;
        }else if(findLable(headOfTable, param) != NULL){
            if(commandFound->funct != 0){
                int *functInBinary, i, j;
                functInBinary = decToBinary(commandFound->funct);
                for(i = 12, j = 0; i < 16; i++, j++)
                    link->word[i] = functInBinary[j];
            }
            link->word[0] = 1;
            link->word[18] = 1;
            IC = IC + 3;
        }else{
            char *registerName;
            int regNum;
            int *regInBin, i, j;
            if(commandFound->funct != 0){
                int *functInBinary;
                functInBinary = decToBinary(commandFound->funct);
                for(i = 12, j = 0; i < 16; i++, j++)
                    link->word[i] = functInBinary[j];
            }
            link->word[1] = 1;
            link->word[18] = 1;
            registerName = strtok(param, "[]");
            regNum = isARegister(registerName);
            regInBin = decToBinary(regNum);
            for(j = 0, i = 2; i < 6; i++, j++)
                link->word[i] = regInBin[j];
            addWord(headForLine,link);
            IC = IC + 3;
        }
    }else{
        char *param1, *param2;
        param1 = strtok(NULL, CUT);
        param2 = strtok(NULL, CUT);
        WORD *link;
        if(isARegister(param1)!= -1 && isARegister(param2)!= -1)
        {
            int regSource = isARegister(param1), i = 8, j;
            int regDest = isARegister(param2);
            int *regSourceBin, *regDestBin;
            regSourceBin = desToBinary(regSource);
            for(j = 0; i < 12; i++, j++)
                link->word[i] = regSourceBin[j];
            link->word[0] = 1;
            link->word[1] = 1;
            regDestBin = desToBinary(regDest);
            for(j = 0, i = 2; i < 6; i++, j++)
                link->word[i] = regSourceBin[j];
            link->word[6] = 1;
            link->word[7] = 1;
            if(commandFound->funct != 0){
                int *functInBinary;
                functInBinary = decToBinary(commandFound->funct);
                for(i = 12, j = 0; i < 16; i++, j++)
                    link->word[i] = functInBinary[j];
            }
            link->word[18] = 1;
            addWord(headForLine, link);
            IC++;
            }
		/*case 2 params*/
        }

    }


void firstPass(FILE *filePointer)
{
    int IC = 100;
    int DC = 0;
    char line[81];
    char lineCopy[81];
    char *token;
    symbolLink *lable;
    symbolLink *headOfTable = symboleTableCreat(filePointer);
    LINE *headOfFile;
    
    while(fgets(line, 81, filePointer))
    {
        strcpy(lineCopy, line);
        token = strtok(line, CUT);
        if(isLable(token)){
            lable = find(headOfTable, token);
            lable->adress = IC;
            token = strtok(NULL, '\n');
            /*a method not written yet to code it to binary*/
        }else if(!strcmp(token, ".extern")){
            token = strtok(NULL, CUT);
            lable = find(headOfTable, token);
            lable->visibility = 2;
        }else if(!strcmp(token, ".entry")){
            token = strtok(NULL, CUT);
            lable = find(headOfTable, token);
            lable->visibility = 1;
        }
        /*a method to check if guidance or instruction and code it to binary*/
    }
}


