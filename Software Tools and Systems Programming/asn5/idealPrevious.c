#include "headers.h"
// finds the ideal spot to add the node, given the order rules
LIST_NODE* idealPrevious(LIST* list, LIST_NODE* node)
{
    if(list->head == NULL)
        return NULL;
    else
    {
        // start the search at beginning of list (head)
        LIST_NODE* currNode = list->head;

        // no previous node yet, assign NULL
        LIST_NODE* prevNode = NULL;

        // get the node data for the node we are given
        DOCUMENT* nodeData = node->dataPtr;

        // declare variable to store current node data
        DOCUMENT* currNodeData;

        // loop until end of list
        while(currNode != NULL)
        {
            // get the current node's data
            currNodeData = currNode->dataPtr;
            // check currNode priority
            if(currNodeData->requestPriority == nodeData->requestPriority)
            {
                // if priority is the same, look at page count
                // account for priority 0, we want to put them at the end of the 0s
                if((currNodeData->numPages >= nodeData->numPages) && (nodeData->requestPriority != 0))
                    return prevNode;
                else
                {
                    // node's page count is more than the current page, move to next node
                    prevNode = currNode;
                    currNode = currNode->next;
                }
            }
            // gone too far into a lower priority realm, go back one node
            else if(currNodeData->requestPriority > nodeData->requestPriority)
            {
                return prevNode;
            }
            // currNode priority too high, move to next node
            else // currNodeData->requestPriority < nodeData->requestPriority
            {
                prevNode = currNode;
                currNode = currNode->next;
            }
        }
        // end of list
        return prevNode;
    }
}

