#ifndef _validation_H_
#define _validation_H_

#include "utils.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define parse " \t\n"

int isCurNumOfWords(char buffer[], int a);
int isARegister(char []);
int isACommand(char []);
int isAValidName(char []);
int isAGuidance(char []);
int isACommentOrEmpty(char line []);
int isAMacro(char []); /* not exist */
int skip(char line[]); /* same as isAcommentOEempty*/
LIST *validNames(FILE *fileName, char *nameOfFile);
void insertMa(char [], LIST*);
void insertEx(char [], LIST*);
void insertEn(char [], LIST*);
void insertLD(char [], LIST*);

#endif
