#include "headers.h"

// returns a pointer to a list structure
LIST* createList()
{
    // pointer to new list
    LIST* linkedList;

    // make list pointer point to new allocated memory
    linkedList = (LIST*) malloc (sizeof(LIST));

    // if successfully allocated, assign all the variables
    if(linkedList)
    {
        linkedList->head = NULL;
        linkedList->count = 0;
    }
    return linkedList;
}
