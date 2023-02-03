#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "../include/functions_list.h"
#include "../include/functions_abr.h"
#include "../include/functions_avl.h"
#include "../include/functions_common.h"
#include "../include/consts.h"

int main( int argc, char *argv[] ) {
    int opt, optionIndex = 0;    
    int shortOpts = 0, longOpts = 0, rOption = 0, mOrhOption = 0, pOption = 0, wOption = 0, mOption = 0, hOption = 0;
    int avlOption = 0, abrOption = 0, tabOption = 0, longOption = 0;
    int sortingDirection = ASCENDING;
    int sortingIndex1    = DFLTINDEX1;
    int sortingIndex2    = DFLTINDEX2;    
    int fields = DFLTFIELDS;
    char *sortingMethod  = AVLSORT;    
    char *inputFile = NULL;
    char *outputFile = NULL;
    char *pValue = NULL;
    char *tValue = NULL;
    char *pOrtValue = NULL;    
    node list = NULL;
    bool longOptFound = TRUE;
    extractMethod extract;
    struct Fields maxFields;
    char message[2*MAXLENGTH];
    
    //    check that the program must have at least 4 arguments to run
    if ( argc < 4 ) {
        strcpy( message, "wrong number of options/arguments" );
        sendReturnCode( OPTIONERROR, message );
    }  
    //    lists avl, abr and list options in a, b, c
    static struct option longOptions[] = {
        { "avl", no_argument, NULL, 'a' },
        { "abr", no_argument, NULL, 'b' },        
        { "tab", no_argument, NULL, 'c' }        
    };
    
    // command line options and arguments parsing
    while ( ( opt = getopt_long( argc, argv, "p:t:f:o:hmwr", longOptions, &optionIndex ) ) != -1 ) {       
        //      lists all the options, either a short option or a long option
        switch ( opt ) {
                //          pressure
            case 'p' :
                shortOpts++;
                pOption++;
                pValue = optarg;            
                break;
                //          temperature
            case 't' :
                shortOpts++;            
                tValue = optarg;            
                break;
//          height
            case 'h' :
                shortOpts++;            
                mOrhOption++;
                hOption++;
                break;
//          moisture
            case 'm' :
                shortOpts++;        
                mOrhOption++;    
                mOption++;
                break;
//          wind
            case 'w' :
                shortOpts++;      
                wOption++;      
                break;
//          reverse
            case 'r' :         
                rOption++;
                break;
//          input file
            case 'f' :
                inputFile = optarg;
                break;
            case 'o' :
//          output file
                outputFile = optarg;
                break;
//           avl option
            case 'a' :
                longOpts++;            
                avlOption = opt;
                break;
//          abr option
            case 'b' :
                longOpts++;     
                abrOption = opt;        
                break;
                //          list option
            case 'c' :
                longOpts++;        
                tabOption = opt;     
                break;                                                             //          unknown option resulting in an error
            case '?' :
                strcpy( message, "wrong option\n" );
                sendReturnCode( OPTIONERROR, message );
            default :
                strcpy( message, "wrong option\n" );                
                sendReturnCode( OPTIONERROR, message );
        }
    }
    // different error cases for the options
    if ( shortOpts == 0 ) {
        strcpy( message, "too few options\n" );        
        sendReturnCode( OPTIONERROR, message );        
    }    
    // different error cases for the options
    if ( shortOpts > 1 || longOpts > 1 ) {
        strcpy( message, "too many options\n" );         
        sendReturnCode( OPTIONERROR, message );        
    }
    // different error cases for the options
    if ( longOpts > 0 ) {
        for ( int i = 0; i < argc; i++ ) {
            if ( strcmp( argv[i], "-" ) == 0 || strcmp( argv[i], "--" ) == 0 ) {
                sendReturnCode( OPTIONERROR, NULLMESSAGE );            
            }
            //            sorting method depending on the option chosen by the user
            if ( strstr( argv[i], "--" ) != NULL ) {
                sortingMethod = argv[i];
                if (    !( ( avlOption > 0 && strcmp( sortingMethod, "--avl" ) == 0 )    
                        || ( abrOption > 0 && strcmp( sortingMethod, "--abr" ) == 0 )
                        || ( tabOption > 0 && strcmp( sortingMethod, "--tab" ) == 0 ) ) ) {               
                    longOptFound = FALSE;
                    break;
                }        
            }
        }
    }

    if ( longOptFound == FALSE ) {
        strcpy( message, "wrong long option\n" );         
        sendReturnCode( OPTIONERROR, message );
    }  
    //    check if reverse option has been requested
    if ( rOption > 0 ) {
        sortingDirection = DESCENDING;
    }
    //    check if moisture option has been requested
    if ( mOrhOption > 0 ) {
        maxFields.in = 3;
        maxFields.inter = 3;
        maxFields.out = 3;        
        sortingDirection = DESCENDING;
        extract = ( mOption > 0 ) ? M : H;
    }
    //    check if wind option has been requested
    if ( wOption > 0 ) {
        maxFields.in = 4;
        maxFields.inter = 5;
        maxFields.out = 4;
        extract = W;
    }

    // check arguments for '-p' and '-t' options
    pOrtValue = ( pValue == NULL ) ? tValue : pValue;
    if ( pOrtValue != NULL ) {     
        switch ( atoi( pOrtValue ) ) {
            case 1:
                maxFields.in = ( pOption > 0 ) ? 2 : 4;
                maxFields.inter = 5;
                maxFields.out = 4;
                extract = ( pOption > 0 ) ? P1 : T1;
                break;
            case 2:
                maxFields.in = 2;            
                maxFields.inter = 3;
                maxFields.out = 2;                    
                extract = ( pOption > 0 ) ? P2 : T2;
                break;
            case 3:
                maxFields.in = 3;    
                maxFields.inter = 2;
                maxFields.out = 3;                                  
                extract = ( pOption > 0 ) ? P3 : T3;
                sortingIndex1 = 1;
                sortingIndex2 = 0;                 
                break;
            default:
                strcpy( message, "bad argument for option\n" );                   
                sendReturnCode( OPTIONERROR, message );
        }
    }

    if ( !fopen( inputFile, "r" ) ) {       
        strcpy( message, "input file does not exist\n" );         
        sendReturnCode( INFILEERROR, message );
    }    
//    parseInputFile2( inputFile, outputFile, fields, sortingMethod, sortingDirection, sortingIndex1, sortingIndex2/*, extract*/ );
   parseInputFile( inputFile, outputFile, maxFields, sortingMethod, sortingDirection, sortingIndex1, sortingIndex2, extract );    

    return 0;
}

