#include "headers.h"

// inserts a node into a list
void insertNode(LIST* list, LIST_NODE* node)
{
    // get a pointer to the ideal previous node
    LIST_NODE* prevNode = idealPrevious(list, node);

    // get the data from the given node
    DOCUMENT* doc = node->dataPtr;

    // case of empty list or first node is ideal next node
    // regardless, the given node will become the new head
    if(prevNode == NULL)
    {
        node->next = list->head;
        list->head = node;
    }
    // prevNode is the head node
    else if(prevNode == list->head)
    {
        // get the data from the potential previous node
        DOCUMENT* prevDoc = prevNode->dataPtr;

        // check if node priority is less than head OR...
        // they are equal priority and the given document has a lesser or equal amount of pages
        if((doc->requestPriority < prevDoc->requestPriority) ||
           ((doc->requestPriority == prevDoc->requestPriority) && doc->numPages <= prevDoc->numPages))
        {
            node->next = list->head;
            list->head = node;
        }
        else
        {
            node->next = prevNode->next;
            prevNode->next = node;
        }
    }
    else // some node in the middle or end of list
    {
        node->next = prevNode->next;
        prevNode->next = node;

    }
    // node added, increase list count
    list->count++;
}

