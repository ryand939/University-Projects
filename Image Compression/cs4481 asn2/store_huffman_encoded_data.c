
#include "store_huffman_encoded_data.h"

void store_huffman_encoded_data(char *compressed_file_name_ptr, int image_width, int image_height, int
max_gray_value, int number_of_nodes, struct node *huffman_node, long int length_of_encoded_image_array, unsigned char *encoded_image){
    FILE *file = fopen(compressed_file_name_ptr, "wb");
    if (!file) {
        perror("Couldn't open file.");
        return;
    }

    //width
    fwrite(&image_width, sizeof(image_width), 1, file);
    //height
    fwrite(&image_height, sizeof(image_height), 1, file);
    //max gray
    fwrite(&max_gray_value, sizeof(max_gray_value), 1, file);
    //num huffman nodes
    fwrite(&number_of_nodes, sizeof(number_of_nodes), 1, file);
    //values in huffman node array, handle edge case for if there is only 1 color in image, keep one node to hold color
    fwrite(huffman_node, sizeof(struct node), (number_of_nodes == 0 ? 1 : number_of_nodes), file);
    //length of encoded image array
    fwrite(&length_of_encoded_image_array, sizeof(length_of_encoded_image_array), 1, file);
    //encoded image array
    fwrite(encoded_image, sizeof(unsigned char), length_of_encoded_image_array, file);
    //done
    fclose(file);
}

