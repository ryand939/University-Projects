#include "headers.h"

// prints PAGESPERMINUTE pages off head node/print job every time it is called
void usePrinter(LIST* list)
{
    // print job to complete
    static LIST_NODE* printingDoc;

    // document to print
    DOCUMENT* doc;

    // there is no doc being printed, take the head to print
    if(printingDoc == NULL)
    {
        printingDoc = list->head;
        // head node now resides in this function to print, remove it from list
        pop(list);
    }

    // get the document to print
    doc = printingDoc->dataPtr;

    #if DEBUG_PRINT_PROCESS
        printf("PRINTING - DOCUMENT: %5d PAGE: %5d PRIORITY %d\n\n", doc->documentNumber, doc->numPages, doc->requestPriority);
        printList(list);
    #endif

    // decrement the number of pages by the amount we can print at once
    doc->numPages = doc->numPages - PAGESPERMINUTE;

    // increment the number of cycles for other print jobs
    incrementCycles(list);

    // done printing
    if(doc->numPages <= 0)
    {
        // print about the document's completion
        printf("Print Job Completed - Document Number:%5d   -   Cycle Count:%6d\n", doc->documentNumber, doc->numCyclesInQueue);

        #if DEBUG_PRINT_PROCESS
            printList(list);
        #endif

        // we are done with this print job node, free its memory
        free(printingDoc->dataPtr);
        free(printingDoc);

        // NULL printingDoc indicates we are ready for the next document
        printingDoc = NULL;
    }
}
