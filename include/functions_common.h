#ifndef functions_common_h
#define functions_common_h

#include "consts.h"

int max( int a, int b );
int min( int a, int b );
float maxFloat( float a, float b );
float minFloat( float a, float b );
void replaceChar( char *stringInp, char charInp, char charOut );
int stringToTime( char *dateInp );
char *timeToStruct( int timestamp );
int sendReturnCode( int retCode, char *message );
void parseInputFile( char *inputFile,
                     char *outputFile,
                     struct Fields maxFields,
                     char *sortingMethod,
                     int sortingDirection,
                     int sortingIndex1,
                     int sortingIndex2,
                     extractMethod extract );
void *createFields( sorting sortingMethod, struct Parameters params );
#endif /* functions_common_h */
