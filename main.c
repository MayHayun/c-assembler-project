#include <stdio.h>
#include <string.h>
#define FILE_NAME_MAX_SIZE 40

/* func to check if the ending of the file is valid, returns 1 if it's valid */
int fileEndingValid( char *fileName ){

    int i;
    for( i=0; fileName[i] != 0; i++ ){
        
        if( i != 0 && fileName[i] == '.' ){

            if( strcmp( (fileName+i), ".as") == 0 )
                return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]){

    int i, j;
    int duplicateExists = 0;
    int errorCount = 0;
    char fileName[FILE_NAME_MAX_SIZE];

    if ( argc == 1 ){
        printf("No files were given for the program...\n");
        exit(0);
    }
    
    for( i=1; i < argc; i++){

        strcpy(fileName, argv[i]);
        for( j = 1; j < i; j++ ){

            if( !strcmp(fileName, argv[j]) )
                duplicateExists = 1;
        }

        if( !duplicateExists ){
            if( fileEndingValid(fileName) ){
                
                errorCount += FirstPass(fileName) @@@@@@@@@@@@@@@@@@@@@@
                DO THE BIG BAD THING OF THE WHOLE THING
            }
            else{
                printf("The ending of the file needs to be \".as\"  ...\n");
            }
        }
    }
    if( !errorCount > 0 )
        printf("Compilation failed due to %d errors...", errorCount);

    return errorCount;
}
