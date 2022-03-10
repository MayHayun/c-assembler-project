#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "list.h"
#include "validation.h"

#define parse_words "  \t\n"
#define parse_delivery " ,\t\n["
#define parse_commands " ,\t\n:"


int validation(FILE *fileName, LIST *names){
  int result = 1, lineNumber = 0, wordNumber = 0;
  int commaLegit = 0, neededOperands;
  
  char line[MAX_LINE_LENGTH];
  char lineCopy[MAX_LINE_LENGTH];
  char *token;

  /* for every line */
  while (fgets(line, MAX_LINE_LENGTH, fileName)){
    lineNumber++;
    strcpy(lineCopy, line);
    
    /* the limit of 200 lines is for the makinng of the code */
    /* for every word */
    while( token != NULL && lineNumber <= 200 ){
      
      if( lineNumber == 0 )
        token = strtok(lineCopy, parse_words);
      else
        token = strtok(NULL, parse_words);
      
      wordNumber++;
      /* Label Declaration */
      if( wordNumber == 1 && checkForLabelAtBegining(names, token, lineNumber) == 0 ){
        result = 0;
      }
      /*
      if( checkForLabelAtBegining(names, token, lineNumber) == 1 ){
        check commands
      }
      */
      
      /* macro */
      if( wordNumber == 1 && !strcmp(token,"macro") && isCurNumOfWords( line, 2) == 1 ){
        token = strtok(NULL, parse_words);
        wordNumber++;
        if(wordNumber == 2 && checkForMacroAtSecond(names, token, lineNumber) == 0 ){
          result = 0;
        }
      }
      /* extern */
      if( wordNumber == 1 && !strcmp(token,".extern") && isCurNumOfWords( line, 2) == 1 ){
        token = strtok(NULL, parse_words);
        wordNumber++;
        if(wordNumber == 2 && checkForExternAtSecond(names, token, lineNumber) == 0 ){
          result = 0;
        }
      }

      /* entry */
      if( wordNumber == 1 && !strcmp(token,".entry") && isCurNumOfWords( line, 2) == 1 ){
        token = strtok(NULL, parse_words);
        wordNumber++;
        if(wordNumber == 2 && checkForEntryAtSecond(names, token, lineNumber) == 0 ){
          result = 0;
        }
      }

      /* .data */
      if( !strcmp(token,".data")){
        
        /* isValidData */
        
      }

      /* .string */
      if( !strcmp(token,".string")){

        if( countQuotationMarks(line) != 2 ){
          printf("invalid string in line %d\n", lineNumber);
          result = 0;
        }
        if( result ){
          if( line[giveTheLastNoneWhiteIndex(line)] == '"' ){
            token = strtok(NULL, parse_words);
            if( !token[0] == '"' ){
              printf("invalid string in line %d\n", lineNumber);
              result = 0;
            }
          }
        }
      }

      /* command */
      if( isACommand(token) ){
        neededOperands = howManyOperands( token );
      }
      
    }
  }
  
  return result;
}


/* this function is the first pass that parses the
words and filter the special words words */

/* need to fix a bug - if there is empty line in the input we have an error */
LIST *validNames(FILE *fileName, char *nameOfFile){
  char line[MAX_LINE_LENGTH];
  char lineCopy[MAX_LINE_LENGTH];
  char *token;
  LIST *names = newList();
  
  while (fgets(line, MAX_LINE_LENGTH, fileName)){
    strcpy(lineCopy, line);
    
    if(!skip(line)){
      token = strtok(lineCopy, parse);

      /* macro */
      if( !strcmp(token, "macro") && isCurNumOfWords(line,2)){
        token = strtok(NULL, parse); 
        insertMa(token,names);
      }

      /* extern */
      if( !strcmp(token, ".extern") && isCurNumOfWords(line,2)){
        token = strtok(NULL, parse);
        insertEx(token,names);
      }

      /* entry */
      if( !strcmp(token, ".entry") && isCurNumOfWords(line,2)){
        token = strtok(NULL, parse);
        insertEn(token,names);
      }

      /* label declaration */
      if(token[strlen(token)-1] == ':'){
        token[strlen(token)-1] = '\0';
        insertLD(token,names);
      }
    }
  }
  return names;
}

int checkForLabelAtBegining( LIST *names, char token[], int lineNumber ){

  if(token[strlen(token)-1] == ':'){
    token[strlen(token)-1] = '\0';
    
    if( has( names, token) ){
      if( getNode(names, token)->labDec == 1 ){
        getNode(names, token)->labDec = 0;
        return 1;
      }
    }
    printf("inside checkForLabelAtBegining :( \n");
    printf("invalid label in line: %d \n", lineNumber );
  }
  return 0;
}

int checkForMacroAtSecond( LIST *names, char token[], int lineNumber ){

  if( has(names, token) ){
    if( getNode(names, token)->mac == 1 ){
      getNode(names, token)->mac = 0;
      return 1;
    }
  }
    printf("inside checkFor-Macro-AtBegining :( \n");
    printf("invalid macro in line: %d \n", lineNumber );
  return 0;
}

int checkForExternAtSecond( LIST *names, char token[], int lineNumber ){
    if( has(names, token) ){
    if( getNode(names, token)->ext != 0 ){
      return 1;
    }
  }
    printf("inside checkFor-extern-AtBegining :( \n");
    printf("invalid extern in line: %d \n", lineNumber );
  return 0;
}

int checkForEntryAtSecond( LIST *names, char token[], int lineNumber ){
    if( has(names, token) ){
    if( getNode(names, token)->ent != 0 ){
      return 1;
    }
  }
    printf("inside checkFor-entry-AtBegining :( \n");
    printf("invalid entry in line: %d \n", lineNumber );
  return 0;
}

/* given a command find how many */
int howManyOperands( char commandName[] ){
  if( !strcmp(commandName, "mov") || !strcmp(commandName, "cmp") || !strcmp(commandName, "add") || !strcmp(commandName, "sub") || !strcmp(commandName, "lea") ){
    return 2;
  }

  if( !strcmp(commandName, "rts") || !strcmp(commandName, "stop") )
    return 0;

  if( !strcmp(commandName, "clr") || !strcmp(commandName, "not") || !strcmp(commandName, "inc") || !strcmp(commandName, "dec") || !strcmp(commandName, "jmp") || !strcmp(commandName, "bne") || !strcmp(commandName, "jsr") || !strcmp(commandName, "red") || !strcmp(commandName, "prn") ){
    return 1;
  }

  return -1;
}

int isCommandLegit( char line[] ){
  char *token;
  int flag = 1, commas;

  commas = countCommas(line);
  token = strtok( line, "," );

  while( token != NULL && flag ){
    if( isACommand(token) )
      flag = 0;
    
    token = strtok( line, parse_commands );
  }
    
}

/* func to decide which delivery it is
   the given string is after move to none white!*/
int whichDelivery(char myStr[], LIST *names){
  char tempLine[30];
  char tempLabel[30];
  char tempRegister[30];
  char* token;
  
  /* Delivery 0 */
  if( myStr[0] == '#' && isAIntNum(myStr + 1)){
    return 0;
  }
  /* Delivery 1 */
  if(isAGuidance(myStr)){
    return 1;
  }

  /* Delivery 3 */
  if(isARegister(myStr)){
    return 3;
  }
  
  /* Delivery 2 */
  strcpy(tempLine, myStr);
  token = strtok(tempLine, parse_delivery);
  strcpy( tempLabel, token );
  printf("\nlabel->%s", tempLabel);
 
  token = strtok(NULL, parse_delivery);
  if(token == NULL)
    return -1;
  strcpy( tempRegister, token );
  printf("\nregister->%s", tempRegister);

  if( strlen(tempRegister) == 4 ){
      if( tempRegister[0] == 'r' && tempRegister[1] == '1' && tempRegister[3] == ']' ){
  
          if( tempRegister[2] == '0' || tempRegister[2] == '1' || tempRegister[2] == '2' || tempRegister[2] == '3' ||         tempRegister[2] == '4' ||         tempRegister[2] == '5' ){

            if( has( names, tempLabel) ){
              return 2;
            }
          }
      } 
  }

  else
    return -1;
}

int isAGuidance(char line []){
  LIST *l = newList();
  insert(l,".data");
  insert(l,".string");
//insert(l,".entry");
//  insert(l,".extern");
  return has(l,line);
}
  
int isACommentOrEmpty(char line []){
  int i = 0;
  while(line[i] != '\0'){
    if(isWhiteSpace(line[i]))
      i++;
    else if(line[i] == ';')
      return 1;
    else
      return 0;
  }
  return 1;
}

/* as easy as it sounds */
char* getRidOfFirstChar(char myStr[]){
  return myStr + 1;
}

int isACommand(char line []){
  LIST *l = newList();
  insert(l,"mov");
  insert(l,"cmp");
  insert(l,"add");
  insert(l,"sub");
  insert(l,"lea");
  insert(l,"clr");
  insert(l,"not");
  insert(l,"inc");
  insert(l,"dec");
  insert(l,"jmp");
  insert(l,"bne");
  insert(l,"jsr");
  insert(l,"red");
  insert(l,"prn");
  insert(l,"rts");
  insert(l,"stop");
  return has(l,line);
}

int isARegister(char line []){
  LIST *l = newList();
  insert(l,"r0");
  insert(l,"r1");
  insert(l,"r2");
  insert(l,"r3");
  insert(l,"r4");
  insert(l,"r5");
  insert(l,"r6");
  insert(l,"r7");
  insert(l,"r8");
  insert(l,"r9");
  insert(l,"r10");
  insert(l,"r11");
  insert(l,"r12");
  insert(l,"r13");
  insert(l,"r14");
  insert(l,"r15");
  return has(l,line);
}

/* is name valid or not */
int isNameOk(char line []){
  int i = 0;
  if(!isalpha(line[i]) || isARegister(line) || isACommand(line) || strlen(line) > MAX_LABEL_LENGTH){
    return 0;
  }
  while(line[i] != '\0'){
    if(!isalpha(line[i]) && !isANaturalNum(line[i])){
      return 0;
    }
    i++;
  }
  return 1;
}

/* insert macro */
void insertMa(char token [], LIST* names){
  if(isNameOk(token)){
    if(!has(names,token)){
      insert(names,token);
      names->head->mac++;
    }
  }
}

/* insert extern */
void insertEx(char token [], LIST* names){
  if(isNameOk(token)){
    if(!has(names,token)){
      insert(names,token);
      names->head->ext++;
    }
    else{
      NODE* p = getNode(names,token);
      if(p->mac == 0 && p->ent == 0 && p->labDec == 0){
        p->ext++; 
      }
    }
  }
}

/* insert enrty */
void insertEn(char token [], LIST* names){
  if(isNameOk(token)){
    if(!has(names,token)){
      insert(names,token);
      names->head->ent++;
    }
    else{
      NODE* p =getNode(names,token);
      if(p->mac == 0 && p->ext == 0){
        p->ent++; 
      }
    }
  }
}

/* insert label declaration */
void insertLD(char token [], LIST* names){
  if(isNameOk(token)){
    if(!has(names,token)){
      insert(names,token);
      names->head->labDec++;
    }
    else{
      NODE* p =getNode(names,token);
      if(p->mac == 0 && p->ext == 0 && p->labDec == 0){
        p->labDec++; 
      }
    }
  }
}

