#include "headers.h"

void freeMemory(LIST* list)
{
    // head will be null when list is popped to empty
    while(list->head != NULL)
    {
        // get the head
        LIST_NODE* head = list->head;
        // remove head
        pop(list);
        // free head
        free(head->dataPtr);
        free(head);
    }
    // done freeing all nodes
    free(list);
}