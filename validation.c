#include "validation.h"
#include "utils.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int isAGuidance(char line []){
  LIST *l = newList();
  insert(l,".data");
  insert(l,".string");
  insert(l,".entry");
  insert(l,".extern");
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

/* check if the numbers of words in a given number is the same as a given integer number */
int isCurNumOfWords(char line[], int a){
  if(countWords(line) < a){
    //printf("Missing parameter\n");
  }
  if(countWords(line) > a){
    //printf("Extraneous text after end of command\n");
  }
  if(countWords(line) == a){
    return 1;
  }
  return 0;
}
