#include "headers.h"

// returns EMPTY_QUEUE (0) if empty, 1 if list is not empty
int checkCapacity(LIST* list)
{
    // list head null would indicate there are no nodes in the list
    if(list->head == NULL)
        return EMPTY_QUEUE;
    else
        return 1;
}
