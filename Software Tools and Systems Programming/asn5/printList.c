#include "headers.h"

// prints out the contents of the printer queue/list
void printList(LIST* list)
{
    // make sure the list isn't empty
    if(list->head == NULL)
    {
        printf("printList() error: empty list.");
        return;
    }
    // variable to store the current node
    LIST_NODE* currNode;

    // assign the current node to the head/beginning of the list
    currNode = list->head;

    // pointer to the current node's data
    DOCUMENT* data;

    #if DEBUG_LIST
        printf("Current Printer Queue Size: %d\n", list->count);
    #endif

    // loop until the end of the list
    for(int x = 0; x < list->count; x++)
    {
        // get the current node's data
        data = currNode->dataPtr;

        // print current node data
        printf("Current Printer Queue : DocNum:%4d     NumOfPages:%4d     PriorityLevel: %d     NumOfCycles:%4d\n", data->documentNumber, data->numPages, data->requestPriority, data->numCyclesInQueue);

        // get the next node in the list to print
        currNode = currNode->next;
    }
    printf("END OF LIST\n\n");
}

