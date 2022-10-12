#include "headers.h"

// begins the printer for a set number of iterations
void printer(LIST* list)
{
    // variable to hold random number
    int randNum;

    // begin at document #1
    int docNum = 1;

    // rand seed
    srand(time(0));

    // loop ITERATIONS times
    for(int x = 0; x < ITERATIONS; x++)
    {
        // 1 in 10 chance to make new node
        randNum = (rand() % 10) + 1;

        // 10% chance to make a new print job
        if(randNum == 1)
        {
            // initialize a new document and new node to add
            DOCUMENT* newDoc = createDocument(0, docNum);
            LIST_NODE* newNode = createNode(newDoc);
            #if DEBUG_ADDING
                printf("Adding to Queue - Doc: %d NoPages: %d Priority: %d\n", newDoc->documentNumber, newDoc->numPages, newDoc->requestPriority);
            #endif

            // insert the new print job node into the list
            insertNode(list, newNode);

            // increase the number of recorded documents
            docNum++;
        }

        // if the list isnt empty we can start printing
        if(checkCapacity(list) != EMPTY_QUEUE)
            usePrinter(list);
        else
        {
            #if DEBUG_PRINT_PROCESS
                printf("EMPTY QUEUE - no print jobs currently in queue\n");
            #endif
        }
    }
}