#ifndef _validation_H_
#define _validation_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "list.h"

#define parse " \t\n"
int whichDelivery(char myStr[], LIST *names);
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
int regTenToFifthTeen(char reg []);
int checkForLabelAtBegining( LIST *names, char token[], int lineNumber);
int checkForMacroAtSecond( LIST *names, char token[], int lineNumber );
int checkForExternAtSecond( LIST *names, char token[], int lineNumber );
int checkForEntryAtSecond( LIST *names, char token[], int lineNumber );
int isValidString(char myStr []);
#endif
