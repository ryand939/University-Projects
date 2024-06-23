
#include "lz77_decoding_function.h"

// function to decode a lz77 single token, adds decoded pixels to linear array
// this function assumes that the data is encoded correctly
void decode_token(int offset, int length, unsigned char next_symbol, unsigned char *decoded_pixels, int *decoded_index) {
    // find the start to copy from using the current index and offset
    // start is the index from which we will start copying
    int start = *decoded_index - offset;

    // loop through the pixels for the length given by the token, and add them to the end of the array
    // length can also be ZERO and no pixels will be copied
    for (int i = 0; i < length; i++) {
        int copy_from_index = start + i;
        decoded_pixels[*decoded_index] = decoded_pixels[copy_from_index];
        (*decoded_index)++;
    }
    // now append the next symbol given by the current token, and increment the index
    decoded_pixels[*decoded_index] = next_symbol;
    (*decoded_index)++;
}

void Decode_Using_LZ77(char *in_compressed_filename_Ptr){
    // open file, or dont
    FILE *file = fopen(in_compressed_filename_Ptr, "rb");
    if (!file) {
        perror("Failed to open file.");
        return;
    }

    // retrieve headers from compressed file
    int width, height, num_tokens, max_gray_value;
    unsigned int searching_buffer_size;
    fread(&width, sizeof(int), 1, file);
    fread(&height, sizeof(int), 1, file);
    fread(&max_gray_value, sizeof(int), 1, file);
    fread(&searching_buffer_size, sizeof(unsigned int), 1, file);
    fread(&num_tokens, sizeof(int), 1, file);

    // prepare 3 arrays with info from header
    int *offsets = malloc(num_tokens * sizeof(int));
    int *lengths = malloc(num_tokens * sizeof(int));
    unsigned char *next_symbols = malloc(num_tokens * sizeof(unsigned char));


    // retrieve 3 arrays from compressed file
    fread(offsets, sizeof(int), num_tokens, file);
    fread(lengths, sizeof(int), num_tokens, file);
    fread(next_symbols, sizeof(unsigned char), num_tokens, file);

    // done reading from input file
    fclose(file);

    // linear array to store all pixel data, move to image after
    unsigned char decoded_pixels[width*height];

    // track the current index while decoding
    int decoded_index = 0;

    // get all pixel data into linear array, then put in image after
    for(int x = 0; x < num_tokens; x++) {
        //printf("Decoding token (%d, %d, %d)\n", offsets[x], lengths[x], next_symbols[x]);
        decode_token(offsets[x], lengths[x], next_symbols[x], decoded_pixels, &decoded_index);
    }



    // pgm to create with compressed data
    struct PGM_Image pgm;
    create_PGM_Image(&pgm, width, height, max_gray_value);


    // set pixels in pgm based on decoded data
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pgm.image[i][j] = decoded_pixels[i * width + j];
        }
    }
    // done, now just save file
    char reconstructed_filename[100];
    snprintf(reconstructed_filename, sizeof(reconstructed_filename), "%s.pgm", in_compressed_filename_Ptr);
    save_PGM_Image(&pgm, reconstructed_filename, true);

    free_PGM_Image(&pgm);
    free(offsets);
    free(lengths);
    free(next_symbols);
}
