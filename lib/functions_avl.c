#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/consts.h"
#include "../include/functions_avl.h"
#include "../include/functions_common.h"

bTree createBalancedTree( struct Parameters params ) {
    printf(__FUNCTION__);
    printf("\n");    
    bTree newNode = malloc( sizeof( struct BalancedTreeNode ) );

    if ( newNode == NULL ) {
        return NULL;
    }

    if ( params.extract == P1 ) {
        // station id
        strcpy( newNode->fields[0], params.fields[0] );
        // min pressure
        strcpy( newNode->fields[1], params.fields[1] );
        // max pressure
        strcpy( newNode->fields[2], params.fields[1] );
        // inputs sum
        strcpy( newNode->fields[3], params.fields[1] );
        // records number
        strcpy( newNode->fields[4], "1");
    }
    else
    if ( params.extract == P2 || params.extract == T2 ) {
        // date
        strcpy( newNode->fields[0], params.fields[0] );
        // sum
        strcpy( newNode->fields[1], params.fields[1] );
        // records number
        strcpy( newNode->fields[2], "1");
    }
    else
    if ( params.extract == P3 || params.extract == T3 ) {
        // date
        strcpy( newNode->fields[0], params.fields[1] );
        struct Parameters thisParams;
        for ( int i = 0; i < MAXFIELDS; i++ ) {
            strcpy( thisParams.fields[i], params.fields[i] );
        }
        thisParams.sortingDirection = params.sortingDirection;
        thisParams.sortingIndex1 = params.sortingIndex1;
        thisParams.sortingIndex2 = params.sortingIndex2;
        thisParams.extract = P4;

        newNode->child = createBalancedTree( thisParams );
    }
    else
    if ( params.extract == P4 ) {
        // station id
        strcpy( newNode->fields[0], params.fields[0] );
        // pressure        
        strcpy( newNode->fields[1], params.fields[2] );
    }
    else
    if ( params.extract == T1 ) {
        // station id
        strcpy( newNode->fields[0], params.fields[0] );
        // min temperature
        strcpy( newNode->fields[1], params.fields[2] );
        // max temperature
        strcpy( newNode->fields[2], params.fields[3] );
        // inputs sum
        strcpy( newNode->fields[3], params.fields[1] );
        // records number
        strcpy( newNode->fields[4], "1");
    }
    else
    if ( params.extract == W ) {
        // station id
        strcpy( newNode->fields[0], params.fields[0] );
        // GPS coordinates
        strcpy( newNode->fields[1], params.fields[3] );
        // wind direction
        strcpy( newNode->fields[2], params.fields[1] );
        // wind speed
        strcpy( newNode->fields[3], params.fields[2] );
        // records number
        strcpy( newNode->fields[4], "1");
    }
    else
    if ( params.extract == H ) {
        // station id, GPS coordinates, altitude
        for ( int i = 0; i < params.maxFields.in; i++ ) {
            strcpy( newNode->fields[i], params.fields[i] );
        }
    }
    else
    if ( params.extract == M ) {
        // station id
        strcpy( newNode->fields[0], params.fields[0] );
        // GPS coordinates
        strcpy( newNode->fields[1], params.fields[2] );
        // max moisture
        strcpy( newNode->fields[2], params.fields[1] );
    }

    newNode->rightChild = NULL;
    newNode->leftChild  = NULL;
    newNode->height     = 1;
//    newNode->child  = NULL;    
    return( newNode );
}

bTree existsBalancedTree( bTree list, int *h, struct Parameters params ) {
    bTree current = list;
    char *endPtr;
    int dataIn, dataList;

if ( current == NULL ) return NULL;
       
        if ( params.extract == P1 || params.extract == T1 || params.extract == W
                                  || params.extract == H  || params.extract == M ) {
            dataIn   = atoi( params.fields[params.sortingIndex1] );
            dataList = atoi( current->fields[params.sortingIndex1] );              
        }
        else
        if ( params.extract == P2 || params.extract == T2 ) {
            char *dateIn = params.fields[params.sortingIndex1];
            char *dateList = current->fields[params.sortingIndex1];
            dataIn = stringToTime( dateIn ) ;
            dataList = stringToTime( dateList ) ;
        }
        else
        if ( params.extract == P3 || params.extract == T3 ) {
            char *dateIn = params.fields[params.sortingIndex1];
            char *dateList = current->fields[params.sortingIndex2];
            dataIn = stringToTime( dateIn ) ;
            dataList = stringToTime( dateList ) ;
        }
        else
        if ( params.extract == P4 || params.extract == T4 ) {
            dataIn   = atoi( params.fields[params.sortingIndex2] );
            dataList = atoi( current->fields[params.sortingIndex2] );
        }
        if ( dataIn == dataList ) {
            if ( params.extract == P3 || params.extract == T3 ) {
                struct Parameters thisParams;
                for ( int i = 0; i < MAXFIELDS; i++ ) {
                    strcpy( thisParams.fields[i], params.fields[i] );
                }
                thisParams.sortingDirection = params.sortingDirection;
                thisParams.sortingIndex1 = params.sortingIndex1;
                thisParams.sortingIndex2 = params.sortingIndex2;
                thisParams.extract = P4;  
                current->child = insertBalancedTree( current->child, h, thisParams );
            }    
            else {
                updateBalancedTree( current, h, params );                       
                return( current );                  
            }

        }
        if ( existsBalancedTree( current->leftChild, h, params ) != NULL ) return current->leftChild;     
        return( existsBalancedTree( current->rightChild, h, params ) );            
}

bTree updateBalancedTree( bTree list, int *h, struct Parameters params ) {
    char *endPtr;
    float dataIn, dataList;

    if ( list == NULL ) return NULL;

    if ( params.extract == P1 ) {
        for ( int i = 1; i < 5; i++ ) {
            dataList = strtof( list->fields[i], &endPtr );
            switch ( i ) {
                case 1 :
                    // min pressure
                    dataIn   = strtof( params.fields[i], &endPtr );
                    if ( minFloat( dataIn, dataList ) == dataIn ) {
                        sprintf( list->fields[i], "%.2f", dataIn );
                    }
                    break;
                case 2 :
                    // max pressure
                    if ( maxFloat( dataIn, dataList ) == dataIn ) {
                        sprintf( list->fields[i], "%.2f", dataIn );
                    }
                    break;
                case 3 :
                    // pressure sum
                    sprintf( list->fields[i], "%.2f", dataIn + dataList );
                    break;
                case 4 :
                    // records number
                    sprintf( list->fields[i], "%.0f", dataList + 1 );
                    break;
            }
        }
    }
    else
    if ( params.extract == P2 || params.extract == T2 ) {
        for ( int i = 1; i < 3; i++ ) {
            dataList = strtof( list->fields[i], &endPtr );
            switch ( i ) {
                case 1 :
                    // pressure or temp sum
                    dataIn   = strtof( params.fields[i], &endPtr );
                    sprintf( list->fields[i], "%.2f", dataIn + dataList );
                    break;
                case 2 :
                    // records number
                    sprintf( list->fields[i], "%.0f", dataList + 1 );
                    break;
            }
        }
    }
    else
    if ( params.extract == P3 || params.extract == T3 ) {
        struct Parameters thisParams;
        for ( int i = 0; i < MAXFIELDS; i++ ) {
            strcpy( thisParams.fields[i], params.fields[i] );
        }
        thisParams.sortingDirection = params.sortingDirection;
        thisParams.sortingIndex1 = params.sortingIndex1;
        thisParams.sortingIndex2 = params.sortingIndex2;
        thisParams.extract = P4;        
        list = insertBalancedTree( list, h, thisParams );
    }
    else
    if ( params.extract == T1 ) {
        for ( int i = 1; i < params.maxFields.inter; i++ ) {
            dataList = strtof( list->fields[i], &endPtr );
            switch ( i ) {
                case 1 :
                    // min temp
                    dataIn   = strtof( params.fields[2], &endPtr );
                    if ( minFloat( dataIn, dataList ) == dataIn ) {
                        sprintf( list->fields[i], "%.2f", dataIn );
                    }
                    break;
                case 2 :
                    // max temp
                    dataIn   = strtof( params.fields[3], &endPtr );
                    if ( maxFloat( dataIn, dataList ) == dataIn ) {
                        sprintf( list->fields[i], "%.2f", dataIn );
                    }
                    break;
                case 3 :
                    // sum temp
                    dataIn   = strtof( params.fields[1], &endPtr );
                    sprintf( list->fields[i], "%.2f", dataIn + dataList );
                    break;
                case 4 :
                    // number of records
                    sprintf( list->fields[i], "%.0f", dataList + 1 );
                    break;
            }
        }
    }
    else
    if ( params.extract == W ) {
        for ( int i = 2; i < params.maxFields.inter; i++ ) {
            // we skip GPS coordinates update
            dataList = strtof( list->fields[i], &endPtr );
            switch ( i ) {
                case 2 :
                    // wind direction
                    dataIn   = strtof( params.fields[1], &endPtr );
                    sprintf( list->fields[i], "%.2f", dataIn + dataList );
                    break;
                case 3 :
                    // wind speed
                    dataIn   = strtof( params.fields[2], &endPtr );
                    sprintf( list->fields[i], "%.2f", dataIn + dataList );
                    break;                    
                case 4 :
                    // number of records
                    sprintf( list->fields[i], "%.0f", dataList + 1 );
                    break;
            }
        }
    }
    else
    if ( params.extract == H || params.extract == P4 ) {
        // Nothing to do
    }
    else
    if ( params.extract == M ) {
        // max moisture
        dataList = strtof( list->fields[2], &endPtr );
        dataIn   = strtof( params.fields[1], &endPtr );
        if ( maxFloat( dataIn, dataList ) == dataIn ) {
            sprintf( list->fields[2], "%.0f", dataIn );
        }
    }
    return( list );   
}    

int getHeight( bTree node ) { //calcule l'équilibre
    if( node == NULL) {
        return 0;
    }
    return node->height;
}

int getBalanceFactor( bTree node ) {   
    if( node == NULL ) {
        return 0;
    }
    return getHeight( node->leftChild ) - getHeight( node->rightChild );
}

bTree leftRotate (bTree a) {   
    bTree pivot;
    int eq_a, eq_pivot;

    pivot = a->rightChild;
    a->rightChild = pivot->leftChild;
    pivot->leftChild = a;
    eq_a = a->height;
    eq_pivot = pivot->height;
    a->height = eq_a - max(eq_pivot, 0) - 1;
    pivot->height = min(min(eq_a - 2, eq_a + eq_pivot - 2), eq_pivot - 1);
    a = pivot;
    return a;
}

bTree rightRotate (bTree a) {  
    bTree pivot;
    int eq_a, eq_pivot;

    pivot = a->leftChild;
    a->leftChild = pivot->rightChild;
    pivot->rightChild = a;
    eq_a = a->height;
    eq_pivot = pivot->height;
    a->height = eq_a - min(eq_pivot, 0) + 1;
    pivot->height = max(max(eq_a + 2, eq_a + eq_pivot + 2), eq_pivot + 1);
    a = pivot;
    return a;
}

bTree balanceTree (bTree a) {  
    if (a->height >= 2) {
        if (a->rightChild->height >= 0) {
            return (leftRotate(a));
        }
        else
            return (doubleLeftRotate(a));
    }
    else if (a->height <= -2) {
        if (a->leftChild->height <= 0) {
            return (rightRotate(a));
        }
        else
            return (doubleRightRotate(a));
    }
    return a;
}

bTree doubleRightRotate (bTree a) { 
    a->leftChild = leftRotate(a->leftChild);
    return rightRotate(a);
}

bTree doubleLeftRotate (bTree a) {   
    a->rightChild = rightRotate (a->rightChild);
    return leftRotate(a);
}

bTree insertBalancedTree ( bTree a, int *h, struct Parameters params ) {                           
    float dataIn, dataList;
    char *endPtr;   

    if (a == NULL) {
        *h = 1;
        return createBalancedTree( params );
    }
    dataIn   = strtof( params.fields[params.sortingIndex1], &endPtr );
    dataList = strtof( a->fields[params.sortingIndex1], &endPtr );  

    if (dataIn < dataList) {
        a->leftChild = insertBalancedTree(a->leftChild, h, params );
        *h = - *h;
    }
    else
    if (dataIn > dataList) {
        a->rightChild = insertBalancedTree(a->rightChild, h, params );
    }
    else {
        *h = 0;
        return a;
    }

    if ( *h != 0 ) {
        a->height = a->height + *h;
        a = balanceTree(a);
    }
    if (a->height == 0) {
        *h = 0;
    }
    else {
        *h = 1;
    }
    return a;
}

void displayBalancedTreeNode( bTree list, int maxFields, char *prefix ) {           
    if ( strcmp( prefix, NULLMESSAGE ) != 0 ) {
        printf("%s", prefix );
        printf( SEPARATOR );
    }   
    for ( int i = 0; i < maxFields; i++ ) {
        printf( "%s", list->fields[i]);
        if ( i < maxFields-1 ) printf( SEPARATOR );
    }    
}

void displayBalancedTree( bTree list, int maxFields, extractMethod extract, char *prefix ) {
    if ( list != NULL ) {     
        if ( extract == P3 || extract == T3 ) {     
            char *listPrefix = list->fields[0];
            displayBalancedTree( list->leftChild, maxFields, extract, listPrefix );
            displayBalancedTree( list->child, maxFields, 0, listPrefix );            
            displayBalancedTree( list->rightChild, maxFields, extract, listPrefix );            
        }
        else {
            displayBalancedTree( list->leftChild, maxFields, extract, prefix );
            displayBalancedTreeNode( list, maxFields, prefix );
            printf("\n");
            displayBalancedTree( list->rightChild, maxFields, extract, prefix );
        }
    }     
}