#include "headers.h"

// print all arrays except last array
// begins at myArray, and prints nArr arrays
void printArrays(int *** myArray, int nArr, int columns, int rows)
{
    printf("\n");
    // printing an array nArr times
    for(int z = 0; z < nArr; z++)
    {
        // No numbering required if there is only one array to be printed
        if(nArr != 1)
            printf("Array number: %d\n", z + 1);

        for(int x = 0; x < rows; x++)
        {   // *(*(*(myArray + z) + x) + y) to yield int value
            for(int y = 0; y < columns; y++)
                printf("%5d", myArray[z][x][y]);
            printf("\n");
        }
        printf("\n");
    }


}
