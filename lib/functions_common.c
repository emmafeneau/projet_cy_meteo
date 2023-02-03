#include "../include/functions_common.h"
#include "../include/functions_list.h"
#include "../include/functions_abr.h"
#include "../include/functions_avl.h"
#include "../include/consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

// if a code is found in the program, it stops
int sendReturnCode( int retCode, char *message ) {
    switch ( retCode ) {
        case 0 :
            break;
        case 1 :
            if ( strcmp( message, NULLMESSAGE ) != 0 ) {
                printf( "Error : %s\n", message );
            }            
            break;
        case 2 :
            if ( strcmp( message, NULLMESSAGE ) != 0 ) {
                printf( "Error : %s\n", message );
            }          
            break;
        case 3 :
            if ( strcmp( message, NULLMESSAGE ) != 0 ) {
                printf( "Error : %s\n", message );
            }          
            break;
        default :
            break;                                                    
    }
    exit( retCode );
}

// returns the maximum between two numbers
int max( int a, int b ) {
    return( ( a > b ) ? a : b );
}

// returns the minimum between two integers numbers
int min( int a, int b ) {   
    return( ( a < b ) ? a : b );
}

// returns the maximum between two float numbers
float maxFloat( float a, float b ) {   
    return( ( a > b ) ? a : b );
}
// returns the minimum between two float numbers
float minFloat( float a, float b ) {
    return( ( a < b ) ? a : b );
}

// replace a char in a string
void replaceChar( char *stringInp, char charInp, char charOut ) {
    char *pointer;

    pointer = strchr( stringInp, charInp );
    if ( pointer == NULL ) {
        return;
    }
    *pointer = charOut;
}

// transforms a time string into an integer
int stringToTime( char *dateInp ) {
    struct tm tmInp;

    strptime( dateInp, "%Y-%0m-%0dT%0H%0M%0S", &tmInp );
    tmInp.tm_isdst = 0;
    int len = strlen( dateInp );
    char *offsetFull = &dateInp[len-4];
    char *offsetMin  = &dateInp[len-2];
    int secondsOffset = 0, minutesOffset = 0, hoursOffset = 0;

    hoursOffset  = atoi( offsetFull ) / 100;
    minutesOffset = atoi( offsetMin );
    secondsOffset = SECONDSPERMINUTE * minutesOffset + SECONDSPERHOUR * hoursOffset;

    return( (int) mktime( &tmInp ) - secondsOffset );
}

// transforms an integer into a time structure to handle timezone in gnuplot
char *timeToStruct( int timestamp ) {
    time_t tsPtr = (time_t) timestamp;
    struct tm *timeStruct = NULL;
    char timeStr[MAXLENGTH];

    timeStruct = gmtime( &tsPtr );
    strftime( timeStr, MAXLENGTH, "%F-%H", timeStruct );
    return( timeStr );
}

// read input file end create list
void parseInputFile( char *inputFile,
                     char *outputFile,
                     struct Fields maxFields,
                     char *sortingMethod,
                     int sortingDirection,
                     int sortingIndex1,
                     int sortingIndex2, extractMethod extract ) {                       
    char *message[2*MAXLENGTH];
    FILE *fp = NULL;
    char *line = NULL;
    size_t len = 0;
    char *mapPtr = NULL;
    int fileDesc = open( inputFile, O_RDONLY );
    void *myList = NULL;
    struct Parameters thisParams;
    thisParams.sortingDirection = sortingDirection;
    thisParams.sortingIndex1 = sortingIndex1;
    thisParams.sortingIndex2 = sortingIndex2;
    thisParams.extract = extract;        
    thisParams.maxFields.in = maxFields.in;
    thisParams.maxFields.inter = maxFields.inter;    
    thisParams.maxFields.out = maxFields.out;

    if ( fileDesc < 0 ) {   
        strcpy( message, "unable to open input file\n" );      
        sendReturnCode( INFILEERROR, message );
    }
    struct stat statBuf;
    int err = fstat( fileDesc, &statBuf );    
    if ( err < 0 ) {
        strcpy( message, "unable to get stats for input file\n" );           
        sendReturnCode( INFILEERROR, message );       
    }

    char *ptr = mmap( NULL, statBuf.st_size, PROT_READ, MAP_PRIVATE, fileDesc, 0 );
    if ( ptr == MAP_FAILED ) {
        strcpy( message, "unable to mmap from input file\n" );               
        sendReturnCode( OTHERERROR, message );         
    }
    close( fileDesc );
    mapPtr = ptr;

    const char *separator = SEPARATOR;
    char *strToken = NULL;
    char *endPtr = NULL;  
    char params[MAXFIELDS][MAXLENGTH];
    int argLength = 0;
    node list = NULL;
    tree pTree = NULL;
    bTree blTree = NULL;
    int height = 0;

    char *maxPtr = ptr + statBuf.st_size / sizeof( char );
    char *startPtr = ptr;
    char *buffer = NULL;
    int tmpPtr = 0; 
    int cpt = 0, cpt2 = 0;
    unsigned long start = time(NULL);
    while ( ptr < maxPtr ) {
        if ( *ptr == '\n' ) {           
            if ( buffer != NULL ) {
                free( buffer );            
            } 
            tmpPtr = ptr - startPtr;
            buffer = malloc( tmpPtr * sizeof( char ) );
            memcpy( buffer, startPtr, tmpPtr );
            *(buffer + tmpPtr) = '\0';
            startPtr = ptr + 1;
            strToken = strtok( buffer, separator );
            int i = 0; 

            while ( strToken != NULL && strToken != "\n" && i < maxFields.in ) {                              
                strcpy( params[i], strToken );               
                i++;
                strToken = strtok ( NULL, separator );
            }      
                      
            for ( int i = 0; i < MAXFIELDS; i++ ) {
                strcpy( thisParams.fields[i], params[i] );
            }                       
            if ( strcmp( sortingMethod, TABSORT ) == 0 ) {
                (node) myList;
                myList = insertNode( myList, thisParams );    
            }
            else
            
            if ( strcmp( sortingMethod, ABRSORT ) == 0 ) {
                (tree) myList;
                myList = insertTree( myList, thisParams );                
            }                  
            else
            if ( strcmp( sortingMethod, AVLSORT ) == 0 ) {
                (bTree) myList;
                myList = insertBalancedTree( myList, &height, thisParams );       
            }                
        }

        ptr++;
    }
    unsigned long end = time(NULL); 
    fp = fopen( inputFile, "r");
        if ( fp == NULL ) {
            sendReturnCode( INFILEERROR, NULLMESSAGE );
        }

    remove( outputFile );
    fp = fopen( outputFile, "w" );
    if ( fp == NULL ) {
        strcpy( message, "unable to open input file\n" );         
        sendReturnCode( OUTFILEERROR, message );
    } 
    if ( strcmp( sortingMethod, TABSORT ) == 0 ) {
//        displayList( myList, maxFields.inter, extract, NULLMESSAGE );
        listToFile( myList, fp, maxFields.out, extract, NULLMESSAGE ); 

    }
    else
    if ( strcmp( sortingMethod, ABRSORT ) == 0 ) {
//        displayTree( myList, maxFields.inter, extract, NULLMESSAGE );
        treeToFile( myList, fp, maxFields.out, extract, NULLMESSAGE );         
    }      
    else
    if ( strcmp( sortingMethod, AVLSORT ) == 0 ) {
        displayBalancedTree( myList, maxFields.inter, extract, NULLMESSAGE );
    }         
    fclose( fp );    
}

