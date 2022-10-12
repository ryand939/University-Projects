#include "headers.h"

// removes a node from a linked list
void removeNode(LIST* list, LIST_NODE* nodeForRemoval)
{
    // node to store the previous node
    LIST_NODE* prevNode = NULL;

    // node to store the current node
    LIST_NODE* currNode = list->head;

    // loop until the end of the list
    while(currNode != NULL)
    {
        // found node to remove
        if(currNode == nodeForRemoval)
        {
            // case where it is the head node, pop list
            if(prevNode == NULL)
            {
                pop(list);
            }
            // case where it is some middle or end node
            else
            {
                prevNode->next = currNode->next;
                currNode->next = NULL;
                list->count--;
            }
        }
        // get the next nodes to continue searching
        prevNode = currNode;
        currNode = currNode->next;
    }

}