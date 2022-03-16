#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CUT "\t ,\n"

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
void firstFour(WORD * ourWord);
WORD *firstPass(FILE *filePointer, symbolLink *headOfTable);
void toBinaryCommand(char line[], symbolLink *headOfTable, WORD *headOfFile);
void toBinaryGuidance(char line[], WORD *headOfFile);
void extraWordsToBinary(char *param, WORD *headOfFile, symbolLink *headOfTable);
WORD *charToBinary(char ch);
void addWord(WORD *head, WORD *link);
int isNum(char *str);
int extractRegister(char * param);
int isARegister(char line []);
int *decToBinary(int num);

void trimTrailing(char * str){
    int index = -1;
    int i;

    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index= i;
        }

        i++;
    }
    str[index + 1] = '\0';
}

symbolLink *symboleTableCreat(FILE *filePointer)
{
    char line[81];
    char *token;
    symbolLink *head = (struct symbolLink*)malloc(sizeof(struct symbolLink));
    symbolLink *lableFound = NULL;
    head->next = NULL;
    while (fgets(line, 81, filePointer))
    {       
        token = strtok(line, CUT);
        if(token[strlen(token)-1] == ':' && findSymbol(head, strtok(token, ":")) == NULL)
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
      link->next = NULL;
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
        if(!strcmp(current->name, lableName))

          return current;
        else
          current = current->next;
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

void deliveryForBinary(commandsStruct *command ,char myStr[], symbolLink *headOfTable, WORD *headOfFile)
{ 
    int isDest = 1;
    int i, j;
    char* token; 
    WORD* link =(struct WORD*)malloc(sizeof(struct WORD)); 
    token = strtok(myStr, ", \t\n");
    firstFour(link);
    link->next = NULL;
    link->word[18] = 1;
    if(command->funct != 0)
    {
      int *functInBin;
      functInBin = decToBinary(command->funct);
      for(i = 12, j = 0; i < 16; i++, j++)
        link->word[i] = functInBin[j];
    }

    if(command->numOfParam == 0)
      addWord(headOfFile, link);
    else
    {
      printf("first token is->%s@\n", token);
      if(command->numOfParam == 2)
        isDest = 0;
      while(token != NULL)
      {
        /* Delivery 0 */ 
        if( token[0] == '#')
        {
          token = strtok(NULL, ", \t\n");
          continue;
        }

        /* Delivery 1 */ 
        else if(findSymbol(headOfTable, cutWhiteChars(token)) != NULL){
          if(isDest)
            link->word[0] = 1;
          else
            link->word[6] = 1;

        }

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
        token = strtok(NULL, "");
        printf("\ntoken is->%s@\n", token);
        isDest = 1;
      }
    }
    addWord(headOfFile, link);
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
            *(newStr + j) = *(str + i);
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

void toBinaryCommand(char line[], symbolLink *headOfTable, WORD *headOfFile)
{
    char *command, *token, *restOfString;
    commandsStruct *commandFound = NULL;
    WORD *link = (struct WORD*)malloc(sizeof(struct WORD));
    char lineCopy[81];
    char lineCopy2[81];
    restOfString = (char*)malloc(81);
    firstFour(link);
    link->next = NULL;

    strcpy(lineCopy, line);
    strcpy(lineCopy2, line);
    command = strtok(line, CUT);
    commandFound = findCommand(command);
    link->word[commandFound->opcode] = 1;
    link->word[18] = 1;
    addWord(headOfFile, link);
    *(restOfString + strlen(restOfString)) = '\n';
    restOfString = strtok(NULL, command);
    deliveryForBinary(commandFound, restOfString, headOfTable, headOfFile);
    token = strtok(restOfString, ",\n");
    if(token !=NULL &&(!(*(cutWhiteChars(token)) == 'r' && isARegister(cutWhiteChars(token)) != -1)))
          extraWordsToBinary(cutWhiteChars(token), headOfFile, headOfTable);

    if(commandFound->numOfParam == 2)
    {
      char temp[81];
      int i, j = 0, counter = 0;
      for(i = strlen(lineCopy2)-1; i >=0 && lineCopy2[i] != ',' && lineCopy2[i] != ' '; i--)
        counter++;
      for(i = strlen(lineCopy2) - counter; i <strlen(lineCopy2) ;i++, j++)  
        temp[j] = lineCopy2[i];
      if(!(*(cutWhiteChars(temp)) == 'r' && isARegister(cutWhiteChars(temp)) != -1))
          extraWordsToBinary(cutWhiteChars(temp), headOfFile, headOfTable);
    }
    
    /*while(token != NULL)
    {
        if(!(*(cutWhiteChars(token)) == 'r' && isARegister(cutWhiteChars(token)) != -1))
          extraWordsToBinary(cutWhiteChars(token), headOfFile);
        token = strtok(NULL, "\n\t\0 ");
        printf("token is in while->%s@\n", token);
    }*/
}

WORD *firstPass(FILE *filePointer, symbolLink *headOfTable)
{
    char line[81];
    char lineCopy[81];
    char *token;
    char *firstWord;
    symbolLink *lable;
    WORD *headOfFile = (struct WORD*)malloc(sizeof(struct WORD));
    headOfFile->next = NULL;
    
    while(fgets(line, 81, filePointer))
    {
      strcpy(lineCopy, line);
      token = strtok(line, " :\t");

      if((lable = findSymbol(headOfTable, token)) != NULL){
        char *tokenCopy = (char *)malloc(81);
        lable->adress = IC;
        token = strtok(NULL, "\n");
        strcpy(tokenCopy, token);
        firstWord = strtok(token, CUT);
        if(isACommand(firstWord))
          toBinaryCommand(tokenCopy, headOfTable, headOfFile);
        else
          toBinaryGuidance(tokenCopy, headOfFile);
      }else if(!strcmp(token, ".extern"))
      {
        token = strtok(NULL, CUT);
        lable = findSymbol(headOfTable, token);
        lable->visibility = 2;
      }else if(!strcmp(token, ".entry"))
      {
        token = strtok(NULL, CUT);
        lable = findSymbol(headOfTable, token);
        lable->visibility = 1;
      }else if(isACommand(cutWhiteChars(token)))
        toBinaryCommand(lineCopy, headOfTable, headOfFile);
      else
        toBinaryGuidance(lineCopy, headOfFile);
    }
    return headOfFile;
}

void toBinaryGuidance(char line[], WORD *headOfFile)
{
    char *guidWord, *param = NULL;
    guidWord = strtok(line, " \t");
    if(!strcmp(guidWord, ".string"))
    {
      int i;
      param = strtok(NULL, "\"");
      for(i = 0; i < strlen(param); i++)
      {
        addWord(headOfFile, charToBinary(*(param + i)));
        DC++;
      }
      addWord(headOfFile, charToBinary('\0'));
      
    }else{
      param = strtok(NULL, ",");
      while( param != NULL)
      {
        WORD *link = (struct WORD*)malloc(sizeof(struct WORD));
        int *paramInBinary, i, num = 0, k = 1;
        trimTrailing(param);
        firstFour(link);
        for(i = strlen(param)-1; i >= 0; i--)
        {
          if(param[i] == '-')
              num = num * (-1);

          else if( isdigit(param[i]) ){
            num += (param[i] - 48) * k;
            k *= 10;
          }
        }
        paramInBinary = decToBinary(num);
        link->word[18] = 1;
        for(i = 0; i < 16; i++)
          link->word[i] = *(paramInBinary + i);
        addWord(headOfFile, link);
        DC++;
        param = strtok(NULL, ", \n");
      }
    }
}

void extraWordsToBinary(char *param, WORD *headOfFile, symbolLink *headOfTable)
{
    WORD *link = (struct WORD*)malloc(sizeof(struct WORD));
    firstFour(link);
    link->next = NULL;

    if(isNum(param))
    {
      int *numInBinary, i , num = 0, k = 1;
      for(i = strlen(param)-1; i >= 0; i--)
        {
          if(param[i] == '-')
              num = num * (-1);

          else if( isdigit(param[i]) ){
            num += (param[i] - 48) * k;
            k *= 10;
          }
        }
      numInBinary = decToBinary(num);
      for(i = 0; i < 16; i++)
        link->word[i] = *(numInBinary + i);
      link->word[18] = 1;
      addWord(headOfFile, link);
    }else if(findSymbol(headOfTable,cutWhiteChars(param)) != NULL)
    {
      WORD *link1 = (struct WORD*)malloc(sizeof(struct WORD));
      WORD *link2 = (struct WORD*)malloc(sizeof(struct WORD));
      addWord(headOfFile, link1);
      addWord(headOfFile, link2);
    }
}

WORD *charToBinary(char ch)
{
  WORD *link =(struct WORD*)malloc(sizeof(struct WORD));
  int charInAscii = ch, *charInBinary, i;
  firstFour(link);
  link->word[18] = 1;
  charInBinary = decToBinary(charInAscii);
  for(i = 0; i < 16; i++)
      link->word[i] = *(charInBinary + i);
  DC++;
  link->next = NULL;
  return link;
}

void addWord(WORD *head, WORD *link)
{
    while(head->next != NULL)
        head = head->next;
    head->next = link;
    link->next = NULL;
    IC++;
}

int isNum(char *str)
{
    if(str[0] == '#')
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
            *(array + i) = num%2;    
            num = num/2;    
        }  
        return array;
    }

    num = (~num);

    for(i = 0 ; i < 16 ; i ++){    
        *(array + i) = num%2;    
        num = num/2;    
    }

    for(i = 0 ; i < 16 ; i ++){    
        if( *(array + i) == 1 )
            *(array + i) = 0;
        else if( !*(array + i) )
            *(array + i) = 1;
    }  

    return array;
}

void firstFour(WORD * ourWord){
  ourWord->word[19] = 0;
  ourWord->word[18] = 0;
  ourWord->word[17] = 0;
  ourWord->word[16] = 0;
}

int allZero(WORD *link)
{
  int i;
  for( i = 16; i < 20; i++)
    if(link->word[i] != 0)
      return 0;
  return 1;
}

void changeWord(WORD *headOfFile, symbolLink *symbolFound, int counter)
{
  int i;
  int count = 0;;
  for(i = 0; count <= counter*2 && i < IC; i++)
  {
    headOfFile = headOfFile->next;
    if(allZero(headOfFile))
    count++;
  }
  if(symbolFound->visibility == 2)
  {
    headOfFile->word[16] = 1;
    headOfFile->next->word[16] = 1;
  } else
  {
    int offset = symbolFound->adress % 16;
    int base = symbolFound->adress - offset;
    int *offsetInBin, *baseInBin;
    headOfFile->word[17] = 1;
    headOfFile->next->word[17] = 1;
    baseInBin = decToBinary(base);
    for(i = 0; i < 16; i++)
      headOfFile->word[i] = *(baseInBin + i);
    offsetInBin = decToBinary(offset);
    for(i = 0; i < 16; i++)
      headOfFile->next->word[i] = *(offsetInBin + i);
  }
}

void secondPass(FILE *filePointer, WORD *headOfFile, symbolLink *headOfTable)
{
  char line[81];
  char *token;
  int counter = 0;
  symbolLink *symbolFound = NULL;
  while(fgets(line, 81, filePointer))
  {
    token = strtok(line, " \t,");
    printf("all good2\n");
    if(isACommand(token))
    {
      printf("all good3\n");
      token = strtok(NULL, " \t,");
      if((symbolFound = findSymbol(headOfTable, token)) != NULL)
      {
        changeWord(headOfFile, symbolFound, counter);
        counter++;
      }
    }
  }
}

int main(){

    WORD *headOfFile = NULL;
    FILE *fptr;
    FILE *ff /*fp*/;
    symbolLink *head;
    int k, j;
    int bla[20];
    int lines = 0;
    fptr = fopen("t.txt", "r");

    head = symboleTableCreat(fptr);
    fclose(fptr);
    ff = fopen("t.txt", "r");
    
    headOfFile = firstPass( ff, head );
    fclose(ff);

    /*fp = fopen("t.txt", "r");

    secondPass(fp, headOfFile, head);
*/
    headOfFile = headOfFile->next;
    for(k = 19, j = 0; k >= 0; k--, j++)
      bla[k] = j;
    printf("lines b-> ");
    for(k = 0; k < 20; k++)
      printf("%d\t", bla[k]);
    while(headOfFile != NULL)
    {
        int i;
        printf("\nline %d -> ", lines);
        for(i = 19; i >= 0; i--)
        {
            printf("%d\t",headOfFile->word[i]);
        }
        printf("\n");
        lines++;
        headOfFile = headOfFile->next;
    }
    return 1;
}
