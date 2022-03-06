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

      if( !strcmp(token, "macro") ){
        token = strtok(NULL, parse);
        if(isNameOk(token)){
          printf("\n%s@ is a legit macro name", token);
        }
        else{
          printf("\nin line%d: %s is invalid name for a macro..." , i, token);
        }
      }
      
      if( token[strlen(token-1)] ==  ':'){
        printf("\nlabel found!");
      }

      if( !strcmp(token, ".extern")){
        token = strtok(NULL, parse);
        printf("\nextern found:%s@", token);
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
