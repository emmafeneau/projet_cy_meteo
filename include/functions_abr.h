#ifndef functions_abr_h
#define functions_abr_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "consts.h"

tree createTree( struct Parameters params );
tree existsTree( tree list, struct Parameters params );
tree updateTree( tree list, struct Parameters params );
tree insertTree( tree list, struct Parameters params );
void browseAndInsertTree( tree list, tree newNode, struct Parameters params );
void treeToFile( tree list, FILE *fp, int maxFields, extractMethod extract, char *prefix );                 
void displayTreeNode( tree list, int maxFields, char *prefix );
void displayTree( tree pTree, int maxFields, extractMethod extract, char *prefix );    

#endif /* funcions_abr_h */