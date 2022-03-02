#include <stdio.h>
#include <stdlib.h>

typedef struct string
{
    char *data;
}string;

typedef struct commandsStruct{
    char command[30];
    char ARE;
    int opcode;
    int funct;
    int numOfParam;
    char** sourceAble; /* legit source functions */
    char** destAble;   /* legit destination functions */


}commandStruct;

typedef struct commandData{
    char name[10];
    int valid;
    int bytes;

}commandData;

int move_to_none_white(char line[], int i);
