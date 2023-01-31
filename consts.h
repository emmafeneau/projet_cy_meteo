//
//  consts.h
//  projet_meteo
//
//  Created by emmafeneau on 15/01/2023.
//

#ifndef consts_h
#define consts_h

#define ASCENDING 0
#define DESCENDING 1
#define DFLTSORT "--avl"
#define DFLTINDEX1 0
#define DFLTINDEX2 99
#define MAXFIELDS 6
#define MAXLENGTH 20
#define SEPARATOR ';'

struct Node {
    char fields[MAXFIELDS][MAXLENGTH];
    struct Node* next;
    struct Node* previous;
};
typedef struct Node* node;


#endif /* consts_h */
