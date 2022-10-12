#include "headers.h"

void freeMemory(int *** myArray, int nArr, int columns, int rows)
{
    // memory was allocated in a loop so every call must be freed in reverse
    for(int z = 0; z <= nArr; z++)
    {
        for(int x = 0; x < rows; x++)
            free(myArray[z][x]);
        free(myArray[z]);
    }
    free(myArray);
}
