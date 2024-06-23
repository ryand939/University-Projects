#include "generate_pixel_frequency.h"



long int *generate_pixel_frequency(struct PGM_Image *input_pgm_image, int *number_of_non_zero_values_in_the_frequency_array)
{

    long int *pixelFreqArr = calloc(input_pgm_image->maxGrayValue + 1, sizeof(long int));
    if (pixelFreqArr == NULL) {
        fprintf(stderr, "Couldn't allocate memory!\n");
        return NULL;
    }

    *number_of_non_zero_values_in_the_frequency_array = 0;

    // loop through all pixels in the pgm
    for (int i = 0; i < input_pgm_image->height; i++)
        for (int j = 0; j < input_pgm_image->width; j++) {

            // one byte pixels max grey 255
            unsigned char greyValue = input_pgm_image->image[i][j];

            // check if this grey value has never been seen before
            if (pixelFreqArr[greyValue] == 0)
                (*number_of_non_zero_values_in_the_frequency_array)++;

            pixelFreqArr[greyValue]++;

        }
    return pixelFreqArr;
}



