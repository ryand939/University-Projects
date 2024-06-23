
#ifndef LZ77_DECODING_FUNCTION_H
#define LZ77_DECODING_FUNCTION_H
#include "libpnm.h"

void decode_token(int offset, int length, unsigned char next_symbol, unsigned char *decoded_pixels, int *decoded_index);
void Decode_Using_LZ77(char *in_compressed_filename_Ptr);

#endif //LZ77_DECODING_FUNCTION_H
