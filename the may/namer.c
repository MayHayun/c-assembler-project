#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CUT "\t ,\n"
#define CUTme " :\t"

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
    struct WORD *next;
}WORD;

typedef struct LINE
{
    WORD *wordHead;
    struct LINE *next;
}LINE;

typedef struct symbolLink
{
	char name[31];
	int adress;
	int placing; /* 0-code image, 1-data image */
	int visibility; /* 0-internal, 1-entry, 2-extern */
	struct symbolLink *next;

}symbolLink;

symbolLink * symboleTableCreat(FILE *filePointer);
symbolLink *addSymbol(symbolLink *head, char lableName[]);
struct symbolLink* findSymbol(struct symbolLink *head, char lableName[]);

char *cutWhiteChars(char *str);

LINE *firstPass(FILE *filePointer, symbolLink *headOfTable);
LINE *toBinaryCommand(char line[], symbolLink *headOfTable);
LINE *toBinaryGuidance(char line[]);
WORD *extraWordsToBinary(char *param);
WORD *charToBinary(char ch);
void addLine(LINE *head, LINE *link);
void addWord(WORD *head, WORD *link);
int isNum(char *str);
int extractRegister(char * param);
int isARegister(char line []);
int * decToBinary(int num);

symbolLink *symboleTableCreat(FILE *filePointer)
{
    char line[81];
    char *token;
    symbolLink *head = NULL;
    symbolLink *lableFound;
    while (fgets(line, 81, filePointer))
    {       
        token = strtok(line, CUT);
        if(token[strlen(token)-1] == ':')
        {
            head = addSymbol(head, token);
        }
        else if(!strcmp(token, ".extern"))
        {
            token = strtok(NULL, CUT);
            head = addSymbol(head, token);
            head->visibility = 2;
        } else if(!strcmp(token, ".entry"))
        {          
            token = strtok(NULL, CUT);
            lableFound = findSymbol(head, token);
            if(lableFound == NULL)
            {
                head = addSymbol(head, token);
                head->visibility = 1;
            }else
                lableFound->visibility = 1;
        }
    }
    return head;
}

symbolLink *addSymbol(symbolLink *head, char lableName[])
{
    struct symbolLink *link = (struct symbolLink *) malloc (sizeof(struct symbolLink));
    strcpy(link->name, lableName);
    if( head == NULL ){
      return link;
    }
    link->next = head;
    head = link;
    return head;
}

struct symbolLink *findSymbol(struct symbolLink *head, char lableName[]) {
    struct symbolLink* current = head;
    if(head == NULL)
        return NULL;
    while(current != NULL) 
    {   
        if(!strcmp(current->name, lableName))
        {
          return current;
        }
        else{
          current = current->next;
        }
    }
    return NULL;
}

int isACommand(char line[] ){
  if(!strcmp(line,"mov"))
    return 1;
  if(!strcmp(line,"cmp"))
    return 1;
  if(!strcmp(line,"add"))
    return 1;
  if(!strcmp(line,"sub"))
    return 1;
  if(!strcmp(line,"lea"))
    return 1;  
  if(!strcmp(line,"clr"))
    return 1;
  if(!strcmp(line,"not"))
    return 1;
  if(!strcmp(line,"inc"))
    return 1;
  if(!strcmp(line,"dec"))
    return 1;
  if(!strcmp(line,"jmp"))
    return 1;
  if(!strcmp(line,"bne"))
    return 1;  
  if(!strcmp(line,"jsr"))
    return 1;
  if(!strcmp(line,"red"))
    return 1;
  if(!strcmp(line,"prn"))
    return 1;
  if(!strcmp(line,"rts"))
    return 1;
  if(!strcmp(line,"stop"))
    return 1;
  return 0;
}

commandsStruct *findCommand(char * command)
{
    int i = 0;
    for(; i < 16; i++)
    {
        if(!strcmp(command, ourCommands[i].commandName))
        	return &ourCommands[i];
    }
    return NULL;

}

WORD *deliveryForBinary(commandsStruct *command ,char myStr[], symbolLink *headOfTable)
{ 
    int isDest = 1;
    int i, j;
    char* token; 
    WORD* link = NULL; 
    
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
        if( myStr[0] == '#')
        {
            token = strtok(NULL, ", \t\n");
            continue;
        }

        /* Delivery 1 */ 
        else if(findSymbol(headOfTable, myStr) != NULL)
            if(isDest)
                link->word[0] = 1;
            else
                link->word[6] = 1;

        /* Delivery 3 */
        else if(isARegister(cutWhiteChars(token)) != -1)
        {   
            int *regInBinary;
            regInBinary = decToBinary(isARegister(token));
            if(isDest)
            {
                for(i = 2, j = 0; i < 6; i++, j++)
                    link->word[i] = *(regInBinary + j);
                link->word[0] = 1;
                link->word[1] = 1;
            }else
            {
                for(i = 8, j = 0; i < 12; i++, j++)
                    link->word[i] = *(regInBinary + j);
                link->word[6] = 1;
                link->word[7] = 1;
            }
        }

        /*Delivery 2*/
        else
        {
            int reg;
            int *regInBin;
            reg = extractRegister(token);
            regInBin = decToBinary(reg);
            if(isDest)
            {
                for(i = 2, j = 0; i < 6; i++, j++)
                    link->word[i] = *(regInBin + j);
                link->word[1] = 1;
            }else
            {
                for(i = 8, j = 0; i < 12; i++, j++)
                    link->word[i] = *(regInBin + j);
                link->word[7] = 1;
            }
        }
        isDest = 0;
        token = strtok(NULL, ",\n");
    }
    IC++;
    return link;
}

char *cutWhiteChars(char *str)
{
    char *newStr;
    int i, j = 0;
    newStr = malloc(strlen(str) * sizeof(char));
    for(i = 0; i < strlen(str); i++)
    {
        if(!isspace(*(str + i)))
        {
            *(newStr + j) = *(str + j);
            j++;
        }
    }
    *(newStr + j) = '\0';
    return newStr;
}

int move_to_none_white(char *line, int i)
{
    for(; i < strlen(line); i++)
    {
        if(!isspace(line[i]))
            return i;
    }
    return i;
}

LINE *toBinaryCommand(char line[], symbolLink *headOfTable)
{
    char *command;
    char *token;
    WORD *headForLine = (struct WORD*)malloc(sizeof(struct WORD));
    LINE *node = NULL;
    commandsStruct *commandFound;
    char *restOfString;
    node->wordHead = NULL;

    command = strtok(line, CUT);
    commandFound = findCommand(command);
    headForLine->word[commandFound->opcode] = 1;
    headForLine->word[18] = 1;
    IC++;
    node->wordHead = headForLine;
    restOfString = strtok(NULL, "\n");

    addWord( headForLine, deliveryForBinary(commandFound, restOfString, headOfTable));
    
    token = strtok(restOfString, ",\n");

    while(token != NULL)
    {
        if(isARegister(cutWhiteChars(token)) == -1)
            break;
        addWord(headForLine, extraWordsToBinary(token));
        token = strtok(NULL, ",\n");
    }
    return node;
}

LINE *firstPass(FILE *filePointer, symbolLink *headOfTable)
{
    char line[81];
    char lineCopy[81];
    char *token;
    char *firstWord;
    symbolLink *lable;
    LINE *headOfFile;
    headOfFile = NULL;
    
    while(fgets(line, 81, filePointer))
    {
        printf("in while\n");
        strcpy(lineCopy, line);
        token = strtok(line, CUTme);

        if((lable = findSymbol(headOfTable, token)) != NULL){
            char *tokenCopy = NULL;
            lable->adress = IC;
            token = strtok(NULL, "\n");
            strcpy(tokenCopy, token);
            firstWord = strtok(token, CUT);

            if(isACommand(cutWhiteChars(firstWord)))
                if(IC == 100){	    
                    headOfFile = toBinaryCommand(tokenCopy, headOfTable);
		}
                else
                    addLine(headOfFile, toBinaryCommand(tokenCopy, headOfTable));
            else
                if(IC == 100){
                    
                    headOfFile = toBinaryGuidance(tokenCopy);
                  }
                else
                    addLine(headOfFile, toBinaryGuidance(tokenCopy));
        }else if(!strcmp(token, ".extern")){
            token = strtok(NULL, CUT);
            lable = findSymbol(headOfTable, token);
            lable->visibility = 2;

        }else if(!strcmp(token, ".entry")){
            token = strtok(NULL, CUT);
            lable = findSymbol(headOfTable, token);
            lable->visibility = 1;

        } else if(isACommand(cutWhiteChars(token)))
            if(IC == 100){
              
              headOfFile = toBinaryCommand(lineCopy, headOfTable);
            }
            else
              addLine(headOfFile, toBinaryCommand(lineCopy, headOfTable));
        else
            if(IC == 100){
              headOfFile = toBinaryGuidance(lineCopy);
              printf("ice cream\n");
            }
            else
              addLine(headOfFile, toBinaryGuidance(lineCopy));
    }
    
    return headOfFile;
}

LINE *toBinaryGuidance(char line[])
{
    char *guidWord = NULL, *param = NULL;
    WORD *headForLine;
    LINE *node = NULL;
    node->wordHead = NULL;
    guidWord = strtok(line, " \t");

    printf("pancake\n");
    if(!strcmp(guidWord, ".string"))
    {
        int i;
        param = strtok(NULL, "\"");
        param = strtok(NULL, "\"");
        headForLine = charToBinary(*(param));
        node->wordHead = headForLine;
        for(i = 1; i < strlen(param); i++)
            addWord(headForLine, charToBinary(*(param + i)));
    }else{
        int count = 1;
        while(param != NULL)
        {
            int *paramInBinary, i, num, k = 1;
            WORD *link = NULL;
            param = strtok(NULL, ",");
            for(i = strlen(param)-1; i >= 0; i--)
            {
                if(*(param + i) == '-')
                    num = num * (-1);
                else if(isdigit(*param + i)){
                    num = num + (*(param + i) * k);
                    k = k * 10;
                }
            }
            paramInBinary = decToBinary(num);
            link->word[18] = 1;
            for(i = 0; i < 16; i++)
                link->word[i] = *(paramInBinary + i);
            if(count)
                headForLine = link;
            else
                addWord(headForLine, link);
            count++;
            DC++;
        }
    }
    return node;
}

WORD *extraWordsToBinary(char *param)
{
    WORD *link = NULL;
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

WORD *charToBinary(char ch)
{
    WORD *link = NULL;
    int charInAscii = ch, *charInBinary, i;
    link->word[18] = 1;
    charInBinary = decToBinary(charInAscii);
    for(i = 0; i < 16; i++)
        link->word[i] = *(charInBinary);
    DC++;
    return link;

}

void addLine(LINE *head, LINE *link)
{
    while(head->next != NULL)
        head = head->next;
    head->next = link;
    link->next = NULL;
}

void addWord(WORD *head, WORD *link)
{
    while(head->next != NULL)
        head = head->next;

    head->next = link;
    link->next = NULL;
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

int isARegister(char line [])
{
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

int *decToBinary(int num){
    int *array, i;
    array = calloc(16, sizeof(int));

    if( num >= 0 ){
        for(i = 0 ; i < 16 ; i ++){    
            array[i] = num%2;    
            num = num/2;    
        }  
        return array;
    }

    num = (~num);

    for(i = 0 ; i < 16 ; i ++){    
        array[i] = num%2;    
        num = num/2;    
    }

    for(i = 0 ; i < 16 ; i ++){    
        if( array[i] == 1 )
            array[i] = 0;
        else if( !array[i] )
            array[i] = 1;
    }  

    return array;
}

int main(){
    LINE *headOfFile = NULL;
    WORD *link;
    FILE *fptr;
    FILE *ff;
    symbolLink *head;
    fptr = fopen("t.txt", "r");

    head = symboleTableCreat(fptr);
    printf("BOOM\n");
    fclose(fptr);
    ff = fopen("t.txt", "r");
    /*fseek(fptr, 0, SEEK_SET);*/
    headOfFile = firstPass( ff, head );
    
    if( headOfFile != NULL )
      link = headOfFile->wordHead;

    while(headOfFile != NULL)
    {
        int i;
        while(link != NULL)
        {
        for(i = 0; i < 20; i++)
        {
            printf("\t%d\t\n", link->word[i]);
        }
        link = link->next;
        }
        headOfFile = headOfFile->next;
    }
    return 1;
}
