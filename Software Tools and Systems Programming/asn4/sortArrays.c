void sortArrays(int ** myArray, int columns, int rows) {

    // declare a 1D array to store values
    int arr[rows * columns];

    // variable to keep the index while traversing
    int index = 0;

    // temp variable to store integers while sorting
    int temp;

    // put the elements in a 1D array for much easier sorting
    for (int x = 0; x < rows; x++)
        for (int y = 0; y < columns; y++)
        {
            arr[index] = myArray[x][y];
            index++;
        }

    // sort the 1D array
    for (int x = 0; x < (rows * columns); x++)
        for (int y = 0; y < (rows * columns); y++)
            if (arr[x] < arr[y])
            {
                temp = arr[x];
                arr[x] = arr[y];
                arr[y] = temp;
            }

    // replace old values with sorted values
    index = 0;
    for(int x = 0; x < rows; x++)
    {
        for(int y = 0; y < columns; y++)
        {
            myArray[x][y] = arr[index];
            index++;
        }
    }





}