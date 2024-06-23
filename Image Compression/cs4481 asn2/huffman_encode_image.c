
#include "huffman_encode_image.h"


// function that appends my codes to the unsigned char array
void appendCodeToByteArray(unsigned char *encodedImage, unsigned int code, int codeLength, int *currentByteIndex, int *currentBitPosition) {
    // have only one bit set at most significant bit of code
    unsigned int mask = 1 << (codeLength - 1);

    // loop over each bit in the code
    for (int i = 0; i < codeLength; i++) {
        // if current bit is 1, encode 1 in the encoded image
        if (code & mask)
            encodedImage[*currentByteIndex] |= (1 << *currentBitPosition);

        (*currentBitPosition)--;

        // if the current byte has been completely filled,
        // move to next byte and reset bit position
        if (*currentBitPosition < 0) {
            (*currentByteIndex)++;
            *currentBitPosition = 7;
        }
        // shift right to check next bit
        mask >>= 1;
    }
}

// returns the details about a pixel, given its value
struct pixel_codes *valuesToPixel(int value, struct pixel_codes *pixels, int numPixels)
{
    for(int x = 0; x < numPixels; x++)
        if(value == pixels[x].value)
            return &pixels[x];
    return NULL;
}



void generateHuffmanCode(struct pixel_codes *pixels, int number_of_nodes, struct node *huffman_node)
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

unsigned char *huffman_encode_image(struct PGM_Image *input_pgm_image, struct node *huffman_node, int number_of_nodes, long int *length_of_encoded_image_array)
{
    *length_of_encoded_image_array = 0;

    // represent the different possible pixels and their codes in this struct
    struct pixel_codes pixels[number_of_nodes + 1];
    for(int x = 0; x <= number_of_nodes; x++){
        pixels[x].code = 0;
        pixels[x].value = -1;
        pixels[x].length = 0;
    }

    generateHuffmanCode(pixels, number_of_nodes, huffman_node);


    // determine how much space is needed
    int numberOfNonZeroValues = 0;
    long int * frequencyArray = generate_pixel_frequency(input_pgm_image, &numberOfNonZeroValues);
    int bitsNeeded = 0;
    struct pixel_codes p;
    for(int x = 0; x <= 255; x++)
    {
        // skip if color with value=x does not appear
        if(frequencyArray[x] == 0)
            continue;

        // get the length of bits for the code, multiplied by how many times it appears
        p = *valuesToPixel(x, pixels, number_of_nodes + 1);
        bitsNeeded += p.length * frequencyArray[x];
        //printf("Color with value %d at index %d and length %d appears %ld times so it needs space of %ld bits!\n",
               //p.value, x,p.length, frequencyArray[x], p.length * frequencyArray[x]);
    }


    // get length of image array in BYTEs which may require rounding up
    *length_of_encoded_image_array = (bitsNeeded + 7) / 8;
    // allocate space for the huffman encoding
    unsigned char *huffmanEncoding = malloc(*length_of_encoded_image_array);
    if (!huffmanEncoding) {
        printf("Couldn't allocate memory.");
        return NULL;
    }
    // initialize the allocated memory to 0, could've also just done calloc i think
    memset(huffmanEncoding, 0, *length_of_encoded_image_array);

    // starting at byte 0
    int currentByteIndex = 0;
    // starting at most significant bit in the first byte
    int currentBitPosition = 7;

    // current pixel value
    unsigned char pixelValue;
    // loop through all pixels, get their value,
    // get the data for specific pixels from their value,
    // append the pixel's code to the huffman encoding
    for (int y = 0; y < input_pgm_image->height; ++y) {
        for (int x = 0; x < input_pgm_image->width; ++x) {
            pixelValue = input_pgm_image->image[y][x];
            p = *valuesToPixel(pixelValue, pixels, number_of_nodes + 1);
            appendCodeToByteArray(huffmanEncoding, p.code, p.length, &currentByteIndex, &currentBitPosition);

        }
    }

    free(frequencyArray);
    return huffmanEncoding;
}