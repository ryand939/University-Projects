

#include "huffman_decode_image.h"

// returns the details about a pixel, given its value
struct pixel_codes *valueToPixel(int value, struct pixel_codes *pixels, int numPixels)
{
    for(int x = 0; x < numPixels; x++)
        if(value == pixels[x].value)
            return &pixels[x];
    return NULL;
}


// generates huffman codes and stores them in the pixel_codes struct
void generateHuffmanCodes(struct pixel_codes *pixels, int number_of_nodes, struct node *huffman_node)
{
    // handle case where there is only one color in img
    if(number_of_nodes == 0)
    {
        pixels->length = 1;
        pixels->value = huffman_node->first_value;
    }
    // store the first code and its length, might be needed for 2nd digit in huffman pair
    unsigned int firstCode;
    int firstLen;
    for(int x = number_of_nodes - 1; x >= 0; x--)
    {
        firstCode = 0b0;
        firstLen = 0;
        // check for first existing pixel in the list
        for(int p = 0; p <= number_of_nodes; p++){
            // get the first uninitialized pixel
            if(pixels[p].value == -1) {
                pixels[p].value = huffman_node[x].first_value;
                pixels[p].code = (pixels[p].code << 1);
                pixels[p].length += 1;
                break;
            } // or if the pixel is already initialized, add a zero
            else if(pixels[p].value == huffman_node[x].first_value)
            {
                // gather data for first code, it must be the base for the second
                firstCode = pixels[p].code;
                firstLen = pixels[p].length;
                pixels[p].code = (pixels[p].code << 1);
                pixels[p].length += 1;
                break;
            }
        }
        // now for the second value in the huffman node, ensure it is either a 1 if the first value's code is 0, OR it is the first value code + 1
        for(int p = 0; p <= number_of_nodes; p++){
            if(pixels[p].value == -1 || pixels[p].value == huffman_node[x].second_value)
            {
                pixels[p].value = huffman_node[x].second_value;
                pixels[p].code = firstCode;
                pixels[p].length = firstLen + 1;
                pixels[p].code = (pixels[p].code << 1) | 1; // shift left add new 1 bit
                break;
            }
        }

    }
}

struct TreeNode* createTreeNode(int value, struct TreeNode *left, struct TreeNode *right) {
    struct TreeNode *newNode = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    if (newNode) {
        newNode->value = value;
        newNode->left = left;
        newNode->right = right;
    }
    return newNode;
}

// recursive function to free the huffman tree starting from the root
void freeHuffmanTree(struct TreeNode *node) {
    if (node == NULL)
        return;
    freeHuffmanTree(node->left);
    freeHuffmanTree(node->right);
    free(node);
}

char* codeToString(unsigned int code, int length) {
    // allocate mem for string + null terminator
    char* str = (char*)malloc(length + 1);
    if (str == NULL) {
        return NULL;
    }

    unsigned int mask = 1 << (length - 1);
    for (int i = 0; i < length; i++) {
        str[i] = (code & mask) ? '1' : '0';
        mask >>= 1;
    }

    str[length] = '\0'; // add null terminator to indicate end of string
    return str;
}

struct TreeNode * populateHuffmanTree(int number_of_nodes, struct node *huffman_node, struct pixel_codes *pixels)
{
    struct TreeNode *root = createTreeNode(-1, NULL, NULL);
    if(number_of_nodes == 0 && huffman_node->second_value == -1){
        root->value = huffman_node->first_value;
        return root;
    }
    struct TreeNode *currNode = root;
    int left;
    int right;
    struct pixel_codes *currentPixel;
    char *currCode;
    for(int x = number_of_nodes - 1; x >= 0; x--) {
        // root
        // read the first huffman node to add to tree
        left = huffman_node[x].first_value;
        right = huffman_node[x].second_value;

        // two loops for both pixels to handle
        for(int p = 0; p <= 1; p++)
        {
            // get the details for the pixel we're working with, first if p=0, second otherwise
            currentPixel = (p == 0) ? valueToPixel(left, pixels, number_of_nodes + 1) : valueToPixel(right, pixels, number_of_nodes + 1);
            currCode = codeToString(currentPixel->code, currentPixel->length);
            currNode = root;
            // temp ptr for iteration so i can free currCode when finished
            char *tempCodePtr = currCode;
            for(char bit = *tempCodePtr; bit != '\0'; bit = *++tempCodePtr)
            {
                if(bit == '0')
                {
                    // go left
                    // case left not made yet
                    if(currNode->left == NULL){
                        // make the left node hold the value
                        currNode->left = createTreeNode(currentPixel->value, NULL, NULL);
                        // mark current as internal node
                        currNode->value = -1;
                    }
                    currNode = currNode->left;

                }else
                {
                    // go right
                    if(currNode->right == NULL){
                        // make the left node hold the value
                        currNode->right = createTreeNode(currentPixel->value, NULL, NULL);
                        // mark current as internal node
                        currNode->value = -1;
                    }
                    currNode = currNode->right;
                }
            }
            free(currCode);
        }



    }
    return root;
}


struct PGM_Image *huffman_decode_image( int image_width, int image_height, int max_gray_value, int
number_of_nodes, struct node *huffman_node, long int length_of_encoded_image_array, unsigned char *encoded_image){
    struct pixel_codes pixels[number_of_nodes + 1];
    for(int x = 0; x <= number_of_nodes; x++){
        pixels[x].code = 0;
        pixels[x].value = -1;
        pixels[x].length = 0;
    }

    struct PGM_Image *decodedImage;
    decodedImage = (struct PGM_Image *)malloc(sizeof(struct PGM_Image));
    if(decodedImage == NULL) {
        printf("Couldn't allocate memory");
        return 0;
    }
    create_PGM_Image(decodedImage, image_width, image_height, max_gray_value);


    generateHuffmanCodes(pixels, number_of_nodes, huffman_node);
    // tree made
    struct TreeNode *root = populateHuffmanTree(number_of_nodes,huffman_node, pixels);
    struct TreeNode *currentNode;


    unsigned long bitIndex = 0;


    for (int y = 0; y < image_height; ++y) {
        for (int x = 0; x < image_width; ++x) {
            // start from the root of the tree each time and find the value
            currentNode = root;
            while (currentNode->value == -1) {
                // dont go out of bounds of encoded data
                if (bitIndex >= length_of_encoded_image_array * 8) {
                    break;
                }

                // get the bit and the byte being accessed
                unsigned long byteIndex = bitIndex / 8;
                int bitInByte = 7 - (bitIndex % 8);
                unsigned char bit = (encoded_image[byteIndex] >> bitInByte) & 1;

                // 0 move left, 1 move right
                if (bit == 0) {
                    currentNode = currentNode->left;
                } else {
                    currentNode = currentNode->right;
                }

                bitIndex++;
            }

            // value should not be -1, should be leaf, add value to the image
            if (currentNode->value != -1) {
                decodedImage->image[y][x] = currentNode->value;
            }
        }
    }
    return decodedImage;
}