#include "headers.h"

// pops the front (head) off list
void pop(LIST* list)
{
    // get a pointer to the head node
    LIST_NODE* nodeForRemoval = list->head;

    // make the new head the node after head
    list->head = nodeForRemoval->next;

    // remove link from old head node
    nodeForRemoval->next = NULL;

    // decrease list size count
    list->count--;

}

