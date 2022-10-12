#include "headers.h"

void initializeArrays(int *** myArray, int nArrs, int col, int row)
{
    // establish rand seed
    srand(time(NULL));

    for(int z = 0; z <= nArrs; z++)
    {
        // working with the 2d pointer array which is *(myArray + z)
        myArray[z] = (int **) calloc (row, sizeof(int **));
        for(int x = 0; x < row; x++)
        {
            myArray[z][x] = (int *) calloc (col, sizeof(int *));

            // initialize every element randomly except last array
            // initializing ints at *(*(*(myArray + z) + x) + y) or rather myArray[z][x][y]
            if(z != nArrs )
                for(int y = 0; y < col; y++)
                    myArray[z][x][y]  = rand() % (nArrs*row*col*10) + 1;

        }

    }
}