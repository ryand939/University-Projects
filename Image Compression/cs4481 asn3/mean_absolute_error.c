#include "mean_absolute_error.h"
#include <math.h>

float mean_absolute_error(char *file_name_1_ptr, char *file_name_2_ptr){
    struct PGM_Image pgmImage1, pgmImage2;

    // load both images
    load_PGM_Image(&pgmImage1, file_name_1_ptr);
    load_PGM_Image(&pgmImage2, file_name_2_ptr);

    // ensure they are the same size
    if (pgmImage1.height != pgmImage2.height || pgmImage1.width != pgmImage2.width) {
        return -1;
    }


    // default multiplier 1 if max grays are equal
    float multiplier = 1.0f;

    // multiplier to scale is LARGER_GRAY/SMALLER_GRAY

    if (pgmImage1.maxGrayValue != pgmImage2.maxGrayValue) {
        // case pgm2_max_gray > pgm1_max_gray
        if (pgmImage1.maxGrayValue < pgmImage2.maxGrayValue)
            multiplier = (float)pgmImage2.maxGrayValue / (float)pgmImage1.maxGrayValue;
        // case pgm1_max_gray > pgm2_max_gray
        else
            multiplier = (float)pgmImage1.maxGrayValue / (float)pgmImage2.maxGrayValue;
}


    // calculate MAE with multiplier to scale smaller max gray
    float errorSum = 0;
    float pixelImg1;
    float pixelImg2;

    for (int i = 0; i < pgmImage1.height; i++) {
        for (int j = 0; j < pgmImage1.width; j++) {
            pixelImg1 = pgmImage1.image[i][j];
            pixelImg2 = pgmImage2.image[i][j];

            // scale the smaller max gray pixel up to the larger one
            if (pgmImage1.maxGrayValue < pgmImage2.maxGrayValue)
                pixelImg1 *= multiplier;
            else if (pgmImage1.maxGrayValue > pgmImage2.maxGrayValue)
                pixelImg2 *= multiplier;


            errorSum += fabs(pixelImg1 - pixelImg2);
        }
    }

    // error / W*L = mean absolute error
    return errorSum / ((float)pgmImage1.width * (float)pgmImage1.height);


}