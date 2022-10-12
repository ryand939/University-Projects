#include "headers.h"

// increments the numCyclesInQueue counter for each print job
void incrementCycles(LIST* list)
{
    // begin at pointer to head of list
    LIST_NODE* currNode = list->head;

    // declare pointer to document to use later
    DOCUMENT* doc;

    // loop until end of list
    while(currNode != NULL)
    {
        // assign doc the data in the current node
        doc = currNode->dataPtr;

        // increase the number of cycles
        doc->numCyclesInQueue++;

        #if DEBUG_SHOW_CYCLES
            printf("Increment Cycle - Document: %d Pages: %d Priority: %d Cycle Count: %d\n",
            doc->documentNumber, doc->numPages,doc->requestPriority, doc->numCyclesInQueue);
        #endif

        // check if the print job has been in cycle more than the max cycles
        if(doc->numCyclesInQueue == MAXCYCLES + 1)
        {
            #if DEBUG_SHOW_EXCEEDED
                printf("EXCEEDED CYCLE COUNT - Document: %d Pages: %d Priority: %d Cycle Count: %d\n",
                       doc->documentNumber, doc->numPages, doc->requestPriority, doc->numCyclesInQueue);
            #endif

            // exceeded cycle count, remove this node and replace it with same node priority 0

            // get a pointer to the next node in the list
            LIST_NODE* nextCurrNode = currNode->next;

            // remove the current node from the list
            removeNode(list, currNode);

            // since we just deleted the current node, the next node in the list will take its place
            currNode = nextCurrNode;

            // prepare a new document with the same values as the one we deleted
            DOCUMENT* newDoc = createDocument(doc->numCyclesInQueue, doc->documentNumber);
            newDoc->numPages = doc->numPages;

            // prepare a node with the document we just created
            LIST_NODE* newNode = createNode(newDoc);

            // insert the node we created
            insertNode(list, newNode);

        }
        else
            currNode = currNode->next;
    }
}

