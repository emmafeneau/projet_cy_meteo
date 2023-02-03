#ifndef consts_h
#define consts_h

// constants
#define TMPDIRECTORY "tmp/"
#define OUTPUTDIRECTORY "output/"
#define P3OUTPUTDIRECTORY "output/p3/"
#define T3OUTPUTDIRECTORY "output/t3/"
#define ASCENDING 0
#define DESCENDING 1
#define AVLSORT "--avl"
#define TABSORT "--tab"
#define ABRSORT "--abr"
#define DFLTINDEX1 0
#define DFLTINDEX2 99
#define MAXFIELDS 6
#define MAXLENGTH 23
#define DFLTFIELDS 3
#define SECONDSPERMINUTE 60
#define SECONDSPERHOUR 3600
#define SEPARATOR ";"
#define COMMA ','
#define SEMICOLON ';'
#define CR "\n"
#define DATEXTENSION ".dat"
#define NULLTERMINATOR '\0'
#define NULLMESSAGE ""
#define NODEELEMENT char fields[MAXFIELDS][MAXLENGTH]

// lists the different errors in the program
typedef enum {
    AVL,
    TAB,
    ABR
} sorting;

typedef enum {
    NOERROR,
    OPTIONERROR,
    INFILEERROR,
    OUTFILEERROR,
    OTHERERROR
} error;

typedef enum {
    FALSE,
    TRUE
} bool;

// different data extraction methods that the user can use
typedef enum {
    P1,
    P2,
    P3,
    P4,
    T1,
    T2,
    T3,
    T4,
    W,
    H,
    M
} extractMethod;

struct Fields {
    int in;
    int inter;
    int out;
};

//
struct Parameters {
    NODEELEMENT;
    int sortingDirection;
    int sortingIndex1;
    int sortingIndex2;
    extractMethod extract;   
    struct Fields maxFields;
};

// structure for a link chained
struct ListNode {
    NODEELEMENT;
    struct ListNode *next;
    struct ListNode *previous;
    struct ListNode *child;
};
typedef struct ListNode* node;

// structure for a abr
struct TreeNode {
    NODEELEMENT;
    struct TreeNode* rightChild;
    struct TreeNode* leftChild;
    struct TreeNode* child;    
};
typedef struct TreeNode* tree;

// structure for a avl
struct BalancedTreeNode {
    NODEELEMENT;
    struct BalancedTreeNode* rightChild;
    struct BalancedTreeNode* leftChild;
    struct BalancedTreeNode* child;    
    int height;    
};
typedef struct BalancedTreeNode* bTree;

#endif /* consts_h */
