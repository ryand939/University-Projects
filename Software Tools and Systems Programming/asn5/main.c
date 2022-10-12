#include "headers.h"

int main() {

    // create my linked list/printer queue for print job nodes
    LIST* linkedList = createList();

    // start the printer loop
    printer(linkedList);

    // end comments
    printf("End of Program - * Ryan Daer *\n");
    printf("Number of printer jobs left in queue: %d\n", linkedList->count);

    // free memory used for list
    freeMemory(linkedList);
    return 0;
}
