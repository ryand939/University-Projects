
#include <stdio.h>
#include <string.h>
#include "read_huffman_encoded_data.h"
#include "generate_huffman_nodes.h"
#include "huffman_decode_image.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_comp_file>.comp <output_pgm_file>.pgm\n", argv[0]);
        return 0;
    }

    char *inputFileName = argv[1];
    char *outputFileName = argv[2];
    const char *inputExtension = ".comp";
    const char *outputExtension = ".pgm";

    // input file must be .comp
    if (strlen(inputFileName) < strlen(inputExtension) ||
        strcmp(inputFileName + strlen(inputFileName) - strlen(inputExtension), inputExtension) != 0) {
        fprintf(stderr, "Error: Input file must end with '.comp'. (%s)\n", inputFileName);
        return 0;
    }

    // output file must be .pgm
    if (strlen(outputFileName) < strlen(outputExtension) || strcmp(outputFileName + strlen(outputFileName) - strlen(outputExtension), outputExtension) != 0) {
        fprintf(stderr, "Error: Output file must end with '.pgm'\n");
        return 0;
    }

    struct PGM_Image *pgmImage;
    int image_height;
    int image_width;
    int max_gray_value;
    int number_of_nodes;
    struct node *huffmanNodes;
    long int length_of_encoded_image_array;

    //printf("Doing Q5 stuff\n");
    // get unsigned char array containing encoded image values with code from Q5
    unsigned char * encoded_image = read_huffman_encoded_data(inputFileName, &image_width, &image_height, &max_gray_value, &number_of_nodes, &huffmanNodes, &length_of_encoded_image_array);


    //printf("Doing Q6 stuff\n");
    // get the pgm image with code from Q6
    pgmImage = huffman_decode_image(image_width, image_height, max_gray_value, number_of_nodes, huffmanNodes, length_of_encoded_image_array, encoded_image);
    save_PGM_Image(pgmImage, outputFileName, 1);
    free_PGM_Image(pgmImage);
    free(encoded_image);
    free(huffmanNodes);
}