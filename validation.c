#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

#define parse " \t\n"

int validNames(FILE *fileName, char *nameOfFile){
  
  //char (*usedUp)[];  /* list of used lables and macros */
  //char (*externUsed)[]; /* list of used extens  */
  char line[MAX_LINE_LENGTH];
  char lineCopy[MAX_LINE_LENGTH];
  char *token;
  int i = 0;
  while (fgets(line, MAX_LINE_LENGTH, fileName)){
    i++;
    strcpy(lineCopy, line);
    if(!skip(line)){
      token = strtok(lineCopy, parse);

      /* check for a macro */
      if( !strcmp(token, "macro") ){
        token = strtok(NULL, parse);
        if(isNameOk(token)){
          /*printf("\n%s@ is a legit macro name", token);*/
        }
        else{
          printf("\nin line %d: %s is invalid name for a macro..." , i, token);
        }
      }

      /* extern */
      if( !strcmp(token, ".extern")){
        token = strtok(NULL, parse);
        if(isNameOk(token)){
          printf("\n%s@ is a legit label name", token);
        }
        else{
          printf("\nin line %d: %s is invalid name for a label..." , i, token);
        }
      }

      /* entry */
      if( !strcmp(token, ".entry")){
        token = strtok(NULL, parse);
        if(isNameOk(token)){
          printf("\n%s@ is a legit label name", token);
        }
        else{
          printf("\nin line %d: %s is invalid name for a label..." , i, token);
        }
      }
      /* label declaration */
      if(token[strlen(token)-1] == ':'){
        token[strlen(token)-1] = '\0';
        if(isNameOk(token)){
          printf("\n%s@ is a legit declaration label name", token);
        }
        else{
          printf("\nin line %d: %s is invalid name for a declaration label..." , i, token);
        }
      }
    }
  }
  return 1;
}


int main(){
  FILE *filePtr;
  char fileName[10];
  char temp[10];
  int i;

  fgets(fileName, 10, stdin);
  
  strcpy(temp, fileName);
  temp[strlen(temp)-1] = '\0';
  strcat(temp, ".as");
  filePtr = fopen(temp, "r");
  validNames(filePtr, temp);
  
  return 0;
}
