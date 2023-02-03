#ifndef functions_list_h
#define functions_list_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "consts.h"

node createNode( struct Parameters params );
node existsNode( node list, struct Parameters params );
node updateNode( node list, struct Parameters params );
node insertNode( node list, struct Parameters params );
void displayNode( node list, int maxFields, char *prefix );                 
void displayList (node list, int maxFields, extractMethod extract, char *prefix );
void listToFile( node list, FILE *fp/*char *fileName*/, int maxFields, extractMethod extract, char *prefix );
void displayDate( struct tm timeIn );
void replaceChar( char *stringInp, char charInp, char charOut );
#endif /* functions_list_h */
