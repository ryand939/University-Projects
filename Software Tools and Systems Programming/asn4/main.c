#include "headers.h"

int main()
{

    int nArrs, row, collumn;
    int ***nTables;


    printf("\nEnter the number of arrays (0 to quit): ");
    scanf("%d", &nArrs);

    while(nArrs != 0)
    {
        // get user input
        printf("\nEnter values for Rows, Columns (e.g.2 3)): ");
        scanf("%d %d", &row, &collumn);

        // allocate memory for pointer array
        nTables = (int ***) calloc (nArrs + 1, sizeof(int **));

        // initialize all 2D arrays
        initializeArrays(nTables, nArrs, collumn, row);

        // print full array
        printArrays(nTables, nArrs, collumn, row);

        // combine sum of all array integers into last array
        combineArrays(nTables, nArrs, collumn, row);

        // print last array
        printf("Array of Combined Totals based on %d Arrays", nArrs);
        printArrays(nTables + nArrs, 1, collumn, row);

        // sort last array
        sortArrays(*(nTables + nArrs), collumn, row);

        // print last array
        printf("Array of Combined Totals based on %d Arrays Sorted Ascending", nArrs);
        printArrays(nTables + nArrs, 1, collumn, row);

        // deallocate occupied memory
        freeMemory(nTables, nArrs, collumn, row);

        // get user input
        printf("Enter the number of arrays (0 to quit): ");
        scanf("%d", &nArrs);
    }

}
