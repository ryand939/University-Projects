#include "headers.h"

// returns a pointer to a node structure
LIST_NODE* createNode(DOCUMENT* doc)
{
    // pointer to new node
    LIST_NODE* node;

    // make node pointer point to new allocated memory
    node = (LIST_NODE*) malloc (sizeof(LIST_NODE));

    // if successfully allocated, assign all the variables
    if(node)
    {
        node->dataPtr = doc;
        node->next = NULL;
    }
    return node;
}