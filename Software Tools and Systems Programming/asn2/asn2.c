#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#define ITERATIONS 3
#define DEBUG 0


// assuming the user has the window at least larger than the text outputs, approx 70 spaces wide
void print_array(int arr[], int arr_size, int upper_range)
{
    // determine how much spacing is needed
    int space_factor = 1;
    // max characters i want to print per row
    int max_chars = 70;
    //current number of characters printed in the current row
    int char_count = 0;

    // determine how digits for proper printing
    while(upper_range != 0)
    {
        upper_range /= 10;
        space_factor++;
    }

    // prints each number in the array and creates newlines as needed
    for(int x = 0; x < arr_size; x++)
    {
	if(char_count >=  max_chars)
	{
	   printf("\n");
	   char_count = 0;
	}
	printf("%*d", space_factor, arr[x]);
	char_count += space_factor;
   }

}

int main(void)
{
    // provided code for rand seed
    srand(time(NULL));

    // display assignment information
    printf("Assignment Two\n\n");

    // temp var placeholder for sorting
    int temp;

    // generate random size for array. minimum 50, max (ITERATIONS*50)
    int array_size = rand() % ((ITERATIONS * 50) - 49) + 50;

    // relevant array size after duplicate values are removed
    int modified_array_size = array_size;

    // the array of random size we will work with
    // I chose int as opposed to unsigned long long due to the size limitations of RAND_MAX
    int my_array[array_size];

    // upper range for random numbers to be assigned positions in my_array
    int upper_range;

    printf("  Value of random size of array: %d\n  Size of array: %d bytes\n\n", array_size, sizeof(int)*array_size);

    // main loop. This will run once per iteration.
    for(int x = 1; x <= ITERATIONS; x++)
    {
        upper_range = (int)pow(10, ITERATIONS - (x - 1));

        printf("THIS IS ITERATION NUMBER: %d OF %d\n", x, ITERATIONS);
        #if DEBUG
            printf("Upper Range Value: %d\n", upper_range);
        #endif

        //reset the modified array from last iteration
        modified_array_size = array_size;

        // populate array with values based on current iteration
        for(int i = 0; i < array_size; i++)
            my_array[i] = (rand() % upper_range) + 1;

        // print the new array
        printf("\n  This is the original array populated with values in the range of 1 and %d\n", upper_range);
        printf("  Number of elements in the original array is: %d\n\n", array_size);
        print_array(my_array, modified_array_size, upper_range);
        printf("\n\n");

        // duplicate remover code
        for(int y = 0; y < modified_array_size; y++)
        {
            for(int j = 0; j < modified_array_size; j++)
            {
                if((y != j) && (my_array[y] == my_array[j]))
                {
                    #if DEBUG
                        printf("The value of %d at array index [%d] and the value of %d at array index [%d] are the same.\n", my_array[y], y, my_array[j], j);
                    #endif
                    // shift everything left 1
                    for(int p = j; p < (array_size - 1); p++)
                        my_array[p] = my_array[p + 1];
                    // adjust for new size
                    modified_array_size--;
                    // set the last index to 0
                    my_array[modified_array_size] = 0;
                    // check same index again in case there are two of the same numbers in a row
                    j -= 1;
                }
            }
        }

        // print status that duplicates were removed, then print the array in its current state
        printf("\n  This is the current state of the array with all duplicate values removed\n");
        printf("  Number of unique (non-zero, non-duplicate) elements in the array is: %d\n\n", modified_array_size);
        print_array(my_array, modified_array_size, upper_range);
        printf("\n\n\n");

        // sort array code
        for(int j = 0; j < modified_array_size; j++)
        {
            for(int y = 0; y < modified_array_size; y++)
            {
                if(my_array[j] < my_array[y])
                {
                    temp = my_array[j];
                    my_array[j] = my_array[y];
                    my_array[y] = temp;
                }
            }
        }

        // print status that the array has been sorted, then print the sorted array
        printf("  These are the unique, non-zero elements in the array sorted in ascending order: \n\n");
        print_array(my_array, modified_array_size, upper_range);


        printf("\n--------------------------------------------------\n\n");

    }

    return 0;
}
