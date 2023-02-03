#ifndef functions_avl_h
#define functions_avl_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "consts.h"

bTree createBalancedTree( struct Parameters params );
bTree existsBalancedTree( bTree list, int *h, struct Parameters params );
bTree updateBalancedTree( bTree list, int *h, struct Parameters params );
//bTree createAvl( char params[MAXFIELDS][MAXLENGTH] );
//int max( int a, int b );
//int min( int a, int b );
int getHeight( bTree node );
int getBalanceFactor( bTree node );
bTree rightRotate( bTree node );
bTree leftRotate( bTree node );
bTree balanceTree (bTree a);
bTree doubleRightRotate (bTree a);
bTree doubleLeftRotate (bTree a);                    
bTree insertBalancedTree( bTree node, int *h, struct Parameters params );  
void displayBalancedTreeNode( bTree list, int maxFields, char *prefix );     
void displayBalancedTree( bTree list, int maxFields, extractMethod extract, char *prefix );                 
#endif /* functions_avl_h */
