#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define CUT "\t\n "
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
/*char * getWord(char *word, char line[], int i);*/
MACRO *push(char *macroName, char *macroCommands[], int numOfCommands);



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
    char lineCopy[MAX_LINE_LENGTH];
    char *token = NULL;
    char macro[] = "macro\0";
    MACRO *head = NULL;
    MACRO *macroFound;
    char macroName[30];
    char *macroCommands[6];
    int inMacro = 0;
    int i = 0, j, linesInMacro = 0;

    FILE *objectFile = fopen(strcat(nameOfFile, ".ob"), "w");
    while (fgets(line, MAX_LINE_LENGTH, fileName))
    {
        strcpy(lineCopy,line);
        printf("\n666\n");
        if(!skip(line)){
            printf("didnt skip\n");
            token = strtok(lineCopy, CUT);
            if(inMacro == 0){
                printf("\ntoken is:%s@", token);
                printf("\nmacro is:%s@", macro);

                if( !strcmp(macro, token) ){
                    printf("\nCCCC");
                    inMacro = 1;
                    token = strtok(NULL, CUT);
                    strcpy(macroName ,token);
		    
                    printf("token is:%s", token);
                } else{
                    printf("\nKKKKKKK");
                    macroFound = find(head, macroName);
                    if(macroFound != NULL){
                        printf("7\n");
                        for(j = 0; j < macroFound->numOfCommands; j++)
                        {
                            fputs(macroFound->macroCommands[j], objectFile);
                        }
                    }else
                        printf("\nline is:%s\n", line);
                        i = fputs(line, objectFile);
                }
            } else {
                printf("\n\naaaaaaaa\n");
                if (!strcmp(token, "endm"))
                {
		    printf("\n\njesus");
                    /*head = push(macroName, macroCommands, linesInMacro);*/
                    inMacro = 0;
                    linesInMacro = 0;
                } else{
                    /*strcpy(macroCommands[linesInMacro], line);
                    linesInMacro++;*/
                }
            }
        }
    }  
}


struct MACRO* find(struct MACRO *head, char * macroName) {
    struct MACRO* current = head;
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

/*char * getWord(char *word, char line[], int i){
    int j;
    for(j = 0; i < MAX_LINE_LENGTH && isspace(line[i])==0 ; j++){
        word[j] = line[i];
        i++;
        printf("\np");
    }
    printf("\n in get word\n");
    printf("\n%s\n", word);
    word[j] = '\0';
    return word;
}*/

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
