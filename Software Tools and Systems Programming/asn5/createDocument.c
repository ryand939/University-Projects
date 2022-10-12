#include "headers.h"

// returns a pointer to a document structure
DOCUMENT* createDocument(int numCyclesInQueue, int documentNumber)
{
    // priority of new document
    int priority;

    // number of pages in new document
    int numPages;

    // pointer to document to build and return
    DOCUMENT* rtnData;

    // determine request priority
    int myNumber = ((rand() % 100) + 1);
    if(numCyclesInQueue > MAXCYCLES)
        priority = ELEVATED_PRIORITY; // priority 0 because exceed MAXCYCLES
    else if(myNumber <= 10)
        priority = HIGH_PRIORITY; // numbers 1-10
    else if(myNumber <= 30)
        priority = LOW_PRIORITY; // numbers 11-30
    else
        priority = AVERAGE_PRIORITY; // numbers 31-100

    //numPages is between 1 and MAXPAGES inclusive
    numPages = (rand() % MAXPAGES) + 1;

    // allocate memory for document
    rtnData = (DOCUMENT*) malloc (sizeof(DOCUMENT));

    // if successfully allocated, assign all the variables
    if(rtnData)
    {
        rtnData->documentNumber = documentNumber;
        rtnData->requestPriority = priority;
        rtnData->numPages = numPages;
        rtnData->numCyclesInQueue = numCyclesInQueue;
    }

    return rtnData;
}

