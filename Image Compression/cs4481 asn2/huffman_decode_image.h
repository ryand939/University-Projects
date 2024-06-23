

#ifndef HUFFMAN_DECODE_IMAGE_H
#define HUFFMAN_DECODE_IMAGE_H

#include "read_huffman_encoded_data.h"


struct pixel_codes {int value; unsigned int code; int length;};
struct TreeNode {int value; struct TreeNode *left; struct TreeNode *right;};
char* codeToString(unsigned int code, int length);
void freeHuffmanTree(struct TreeNode *node);
struct TreeNode* createTreeNode(int value, struct TreeNode *left, struct TreeNode *right);
struct TreeNode * populateHuffmanTree(int number_of_nodes, struct node *huffman_node, struct pixel_codes *pixels);
struct PGM_Image *huffman_decode_image( int image_width, int image_height, int max_gray_value, int
number_of_nodes, struct node *huffman_node, long int length_of_encoded_image_array, unsigned char *encoded_image);

#endif //HUFFMAN_DECODE_IMAGE_H
