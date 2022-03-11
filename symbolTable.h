#include <stdio.h>

#define CUT "\t ,"

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
