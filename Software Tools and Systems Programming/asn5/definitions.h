#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

    #define PAGESPERMINUTE 1 // number of pages to print per printer iteration
    #define MAXCYCLES 200 // maximum cycles a document will run before receiving elevated priority
    #define MAXPAGES 30 // maximum pages in any document
    #define ITERATIONS 1000 // number of times the program will run

    #define ELEVATED_PRIORITY 0
    #define HIGH_PRIORITY 1
    #define AVERAGE_PRIORITY 2
    #define LOW_PRIORITY 3

    #define EMPTY_QUEUE 0

    #define DEBUG_LIST 0
    #define DEBUG_ADDING 0
    #define DEBUG_PRINT_PROCESS 0
    #define DEBUG_SHOW_CYCLES 0
    #define DEBUG_SHOW_EXCEEDED 0


    // ADT Type Definitions
    typedef struct node
    {
        void* dataPtr; // pointer to document structure
        struct node* next; // must be a singly linked list
    } LIST_NODE;

    typedef struct
    {
        LIST_NODE* head; // pointer to the beginning of the linked list
        int count; // number of nodes in the linked list
    } LIST;

    typedef struct document
    {
        int documentNumber;
        int requestPriority;
        int numPages;
        int numCyclesInQueue;
    } DOCUMENT;


    // function prototypes
    LIST* createList();
    LIST_NODE* idealPrevious(LIST*, LIST_NODE*);
    LIST_NODE* createNode(DOCUMENT*);
    DOCUMENT* createDocument(int, int);
    void printList(LIST*);
    void insertNode(LIST*, LIST_NODE*);
    void pop(LIST*);
    void usePrinter(LIST*);
    void incrementCycles(LIST*);
    void printer(LIST*);
    void removeNode(LIST*, LIST_NODE*);
    void freeMemory(LIST*);
    int checkCapacity(LIST*);

#endif //DEFINITIONS_H_INCLUDED
