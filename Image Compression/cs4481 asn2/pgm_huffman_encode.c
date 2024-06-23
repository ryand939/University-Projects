
#include <stdio.h>
#include <string.h>
#include "generate_pixel_frequency.h"
#include "store_huffman_encoded_data.h"
#include "huffman_encode_image.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_pgm_file>.pgm <output_file>.comp\n", argv[0]);
        return 0;
    }

    char *inputFileName = argv[1];
    char *outputFileName = argv[2];
    const char *inputExtension = ".pgm";
    const char *outputExtension = ".comp";

    // input file must be .pgm
    if (strlen(inputFileName) < strlen(inputExtension) ||
        strcmp(inputFileName + strlen(inputFileName) - strlen(inputExtension), inputExtension) != 0) {
        fprintf(stderr, "Error: Input file must end with '.pgm'. (%s)\n", inputFileName);
        return 0;
    }

    // output file must be .comp
    if (strlen(outputFileName) < strlen(outputExtension) || strcmp(outputFileName + strlen(outputFileName) - strlen(outputExtension), outputExtension) != 0) {
        fprintf(stderr, "Error: Output file must end with '.comp'\n");
        return 0;
    }


    struct PGM_Image pgmImage;
    int numberOfNonZeroValues = 0;
    long int *frequencyArray;
    // load the PGM image
    if (load_PGM_Image(&pgmImage, inputFileName) != 0) {
        fprintf(stderr, "Could not load the image\n");
        return 0;
    }
    //printf("Doing Q1 code\n");
    // get the frequency array using code from Q1
    frequencyArray = generate_pixel_frequency(&pgmImage, &numberOfNonZeroValues);
    if (frequencyArray == NULL) {
        free_PGM_Image(&pgmImage);
        fprintf(stderr, "Failed to create frequency array.\n");
        return 0;
    }

    //printf("Doing Q2 code\n");
    // generate the huffman nodes using code from Q2
    struct node *huffmanNodes = generate_huffman_nodes(frequencyArray, 255, numberOfNonZeroValues);

    //printf("Doing Q3 code\n");
    // encode image using code from Q3
    long length_of_encoded_array = 0;
    unsigned char *encoded_image = huffman_encode_image(&pgmImage, huffmanNodes, numberOfNonZeroValues-1, &length_of_encoded_array);

    //printf("Doing Q4 code\n");
    // store image using code from Q4
    store_huffman_encoded_data(outputFileName, pgmImage.width, pgmImage.height, pgmImage.maxGrayValue, numberOfNonZeroValues-1, huffmanNodes, length_of_encoded_array, encoded_image);
    free(frequencyArray);
    free(encoded_image);
    free(huffmanNodes);
    free_PGM_Image(&pgmImage);
}
