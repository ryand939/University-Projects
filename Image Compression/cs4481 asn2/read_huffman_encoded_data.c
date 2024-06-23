
#include "read_huffman_encoded_data.h"


unsigned char *read_huffman_encoded_data(char *compressed_file_name_ptr, int *image_width, int *image_height, int
*max_gray_value, int *number_of_nodes, struct node **huffman_node, long int *length_of_encoded_image_array){

    // read in width, height, gray, num huffman nodes, huffman nodes, length of encoded img array, encoded img->rtn

    // open file for READING
    FILE *file = fopen(compressed_file_name_ptr, "rb");
    if (!file) {
        perror("Couldn't open file.");
        return NULL;
    }

    // read in width
    fread(image_width, sizeof(*image_width), 1, file);
    // read in height
    fread(image_height, sizeof(*image_height), 1, file);
    // read in max gray
    fread(max_gray_value, sizeof(*max_gray_value), 1, file);
    // read in num huffman nodes
    fread(number_of_nodes, sizeof(*number_of_nodes), 1, file);
    // allocate mem for huffman nodes, then read in
    *huffman_node = (struct node *)malloc(sizeof(struct node) * ((*number_of_nodes == 0) ? 1 : *number_of_nodes));
    if (!*huffman_node) {
        perror("Couldn't allocate memory for Huffman nodes");
        fclose(file);
        return NULL;
    }
    fread(*huffman_node, sizeof(struct node), ((*number_of_nodes == 0) ? 1 : *number_of_nodes), file);

    // read in len of encoded img array
    fread(length_of_encoded_image_array, sizeof(*length_of_encoded_image_array), 1, file);


    // read in encoded image to return
    // first allocate mem for pointer
    unsigned char *encoded_image = malloc(*length_of_encoded_image_array);
    if (!encoded_image) {
        perror("Couldn't allocate memory");
        fclose(file);
        return NULL;
    }
    // read the rest of the file and store in this pointer
    fread(encoded_image, sizeof(unsigned char), *length_of_encoded_image_array, file);
    fclose(file);
    return encoded_image;

}

