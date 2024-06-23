
#ifndef HUFFMAN_ENCODE_IMAGE_H
#define HUFFMAN_ENCODE_IMAGE_H

#include "generate_huffman_nodes.h"
#include "generate_pixel_frequency.h"
#include <string.h>


struct pixel_codes {int value; unsigned int code; int length;};
void appendCodeToByteArray(unsigned char *encodedImage, unsigned int code, int codeLength, int *currentByteIndex, int *currentBitPosition);
unsigned char *huffman_encode_image(struct PGM_Image *input_pgm_image, struct node *huffman_node, int number_of_nodes, long int *length_of_encoded_image_array);

#endif //HUFFMAN_ENCODE_IMAGE_H
