
#ifndef READ_HUFFMAN_ENCODED_DATA_H
#define READ_HUFFMAN_ENCODED_DATA_H

#include "generate_huffman_nodes.h" // for struct node
unsigned char *read_huffman_encoded_data(char *compressed_file_name_ptr, int *image_width, int *image_height, int
*max_gray_value, int *number_of_nodes, struct node **huffman_node, long int *length_of_encoded_image_array);
#endif //READ_HUFFMAN_ENCODED_DATA_H
