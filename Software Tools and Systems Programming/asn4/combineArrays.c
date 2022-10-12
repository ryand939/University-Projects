void combineArrays(int *** myArray, int nArr, int columns, int rows)
{
    // variable to store the sum of each index
    int currentSum;

    // go through every 2d array except last and sum x and y
    for(int x = 0; x < rows; x++)
        for(int y = 0; y < columns; y++)
        {
            currentSum = 0;
            // sum the current x y for every array
            for(int z = 0; z < nArr; z++)
                currentSum += myArray[z][x][y];

            myArray[nArr][x][y] = currentSum;
        }

}