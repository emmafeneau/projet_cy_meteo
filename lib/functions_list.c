#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/consts.h"
#include "../include/functions_list.h"
#include "../include/functions_common.h"

// functions used for the linked list
// creation of the structure of a node for a linked list
node createNode( struct Parameters params ) {
    node newNode = malloc( sizeof( struct ListNode ) );

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
        printf("create node with date %s\n", newNode->fields[0]);
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

        newNode->child = createNode( thisParams );
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

    newNode->next     = NULL;
    newNode->previous = NULL;  

    return( newNode );
}

//checks if a node exists or not
node existsNode( node list, struct Parameters params ) {
    node current = list;
    char *endPtr;
    int dataIn, dataList;

    while ( current != NULL )  {
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
                current->child = insertNode( current->child, thisParams );
            }    
            else {
                updateNode( current, params );
            }
            return( current );
        }
        current  = current->next;
    }
    return( NULL );
}

// updated a node with the data that are generated
node updateNode( node list, struct Parameters params ) {
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
        list = insertNode( list, thisParams );
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

// insert a node in a link chained
node insertNode( node list, struct Parameters params ) {
    node current = list, previous = NULL, thisNode = NULL, newNode = NULL;
    int dataIn, dataList;
    char *endPtr;
    int listCounter = 0;
    int filter = 0;

    if ( list == NULL ) {
        return( createNode( params  ) );
    }

    if ( existsNode( list, params ) != NULL ) {
        return( list );
    }
    else {       
        newNode = createNode( params );
    }  

    while ( current != NULL )  {
        if ( params.extract == P1 || params.extract == T1 || params.extract == W
                                  || params.extract == H  || params.extract == M ) {
            dataIn   = atoi( newNode->fields[params.sortingIndex1] );
            dataList = atoi( current->fields[params.sortingIndex1] );
        }
        else
        if ( params.extract == P2 || params.extract == T2 ) {
            char *dateIn = params.fields[params.sortingIndex1];
            char *dateList = current->fields[params.sortingIndex1];
            dataIn = stringToTime( dateIn );
            dataList = stringToTime( dateList );
        }
        else
        if ( params.extract == P3 || params.extract == T3 ) {
            char *dateIn = params.fields[params.sortingIndex1];
            char *dateList = current->fields[params.sortingIndex2];
            dataIn = stringToTime( dateIn );
            dataList = stringToTime( dateList );
        }
        else
        if ( params.extract == P4 || params.extract == T4  ) {
            dataIn   = atoi( newNode->fields[params.sortingIndex2] );
            dataList = atoi( current->fields[params.sortingIndex2] );
        }
        if ( params.sortingDirection == ASCENDING) {
            // list end
            if (dataIn > dataList && current->next == NULL){
                current->next = newNode;
                break;
            }
            else
            if (dataIn < dataList){
                newNode->next = current;
                if  (previous == NULL ) {
                    list = newNode;
                    break;
                }
                else {
                    previous->next = newNode;
                    break;
                }
            }
        }
        else
        if ( params.sortingDirection == DESCENDING ) {
            // list start
            if ( dataIn < dataList && current->next == NULL ){
                current->next = newNode;
                break;
            }
            else
            if  ( dataIn > dataList ) {
                newNode->next = current;
                if ( previous == NULL ) {
                    list = newNode;
                    break;
                }
                else {
                    previous->next = newNode;
                    break;
                }
            }
        }
        previous = current;
        current  = current->next;
    }
    return( list );
}

// list is written to a file
void listToFile( node list, FILE *fp, int maxFields, extractMethod extract, char *prefix ) {
    node current = list;
    char *data = malloc( maxFields * MAXLENGTH * sizeof( char ) );
    char *endPtr;

    while ( current != NULL ) {
        if ( extract == P1 || extract == T1 ) {
            // station id or date         
            strcpy( data, current->fields[0] );
            strcat( data, SEPARATOR );
            // average
            float average = strtof( current->fields[3], &endPtr ) / strtof( current->fields[4], &endPtr );
            char avgStr[MAXLENGTH];
            sprintf( avgStr, "%.2f", average );
            strcat( data, avgStr );
            // minimumn, maximum
            for ( int i = 1; i < maxFields-1; i++ ) {
                strcat( data, SEPARATOR );
                strcat( data, current->fields[i] );
            }
            strcat( data, "\n") ;            
        }
        else
        if ( extract == P2 || extract == T2 ) {
            // date
            strcpy( data, current->fields[0] );
            strcat( data, SEPARATOR );
            // average
            float average = strtof( current->fields[1], &endPtr ) / strtof( current->fields[2], &endPtr );
            char avgStr[MAXLENGTH];
            sprintf( avgStr, "%.2f", average );
            strcat( data, avgStr );
            strcat( data, "\n") ;
        }
        else
        if ( extract == P3 || extract == T3 ) {
            // date
            if ( extract == P3 ) {
                listToFile( current->child, fp, maxFields, P4, current->fields[0] );
            }
            else {
                listToFile( current->child, fp, maxFields, T4, current->fields[0] );                
            }
        }          
        else
        if ( extract == P4 || extract == T4 ) {
            // date
            strcpy( data, prefix );
            strcat( data, SEPARATOR );      
            // pressure or temp          
            strcat( data, current->fields[1] );
            strcat( data, "\n") ;
            FILE *stationFp = NULL;
            if ( extract == P4 ) {
                size_t fileSize = strlen( P3OUTPUTDIRECTORY ) + 1
                                + strlen( current->fields[0] ) + 1
                                + strlen( DATEXTENSION ) + 1;
                char *stationFile = malloc( fileSize * sizeof( char) );
                strcpy( stationFile, P3OUTPUTDIRECTORY );            
                strcat( stationFile, current->fields[0] );
                strcat( stationFile, DATEXTENSION );      
                stationFp = fopen( stationFile, "a" );
                if ( stationFp == NULL ) {
                    sendReturnCode( OUTFILEERROR, "unable to open ourput file" );
                }       
                fputs( data, stationFp );
                fclose( stationFp );      
                free( stationFile );                             
            }
            else {
                size_t fileSize = strlen( T3OUTPUTDIRECTORY ) + 1
                                + strlen( current->fields[0] ) + 1
                                + strlen( DATEXTENSION ) + 1;
                char *stationFile = malloc( fileSize * sizeof( char) );
                strcpy( stationFile, T3OUTPUTDIRECTORY );            
                strcat( stationFile, current->fields[0] );
                strcat( stationFile, DATEXTENSION );  
                stationFp = fopen( stationFile, "a" );
                if ( stationFp == NULL ) {
                    sendReturnCode( OUTFILEERROR, "unable to open ourput file" );
                }       
                fputs( data, stationFp );
                fclose( stationFp );      
                free( stationFile );                                
            } 
        }                  
        else
        if ( extract == W ) {
            // GPS coordinates 
            replaceChar( current->fields[1], COMMA, SEMICOLON );            
            strcpy( data, current->fields[1] );     
            // average wind direction, wind speed
            float average;
            char avgStr[MAXLENGTH];
            for ( int i = 2; i < 4; i++ ) {
                strcat( data, SEPARATOR );
                average = strtof( current->fields[i], &endPtr ) / strtof( current->fields[4], &endPtr );
                memset( avgStr, NULLTERMINATOR, sizeof( avgStr ) );
                sprintf( avgStr, "%.2f", average );
                strcat( data, avgStr );                 
            }                   
            strcat( data, "\n") ;         
        }         
        else
        if ( extract == H || extract == M ) {
            // GPS coordinates 
            replaceChar( current->fields[1], COMMA, SEMICOLON );            
            strcpy( data, current->fields[1] );  
            strcat( data, SEPARATOR );
            // Height or max moisture
            strcat( data, current->fields[2] ); 
            strcat( data, "\n\n") ;
        }               
        fputs( data, fp );
        if ( current->next == NULL ) break;
        current = current->next;
    }
}

// display one node
void displayNode( node list, int maxFields, char *prefix ) {        
    if ( strcmp( prefix, NULLMESSAGE ) != 0 ) {
        printf("%s", prefix );
        printf( SEPARATOR );
    }   
    for ( int i = 0; i < maxFields; i++ ) {
        printf( "%s", list->fields[i]);
        if ( i < maxFields-1 ) printf( SEPARATOR );
    }    
}

// display the whole list
void displayList( node list, int maxFields, extractMethod extract, char *prefix ) {
    node current = list;

    while ( current != NULL ) {    
        if ( extract == P3 || extract == T3 ) {         
            displayList ( current->child, maxFields, 0, current->fields[0] );           
        }        
        else {
            displayNode( current, maxFields, prefix );    
            printf("\n");          
        }     
        current = current->next;
    }
}

// display a time struct
void displayDate( struct tm timeIn ) {
    printf(">>> Year : %d, month : %d, day : %d, Hour : %d, minute : %d, second : %d, DS : %d, week day : %d, year day : %d\n",
        timeIn.tm_year+1900, timeIn.tm_mon+1, timeIn.tm_mday, timeIn.tm_hour, timeIn.tm_min, timeIn.tm_sec,timeIn.tm_isdst, timeIn.tm_wday, timeIn.tm_yday );
}
