
#ifndef LZ77_ENCODING_FUNCTION_H
#define LZ77_ENCODING_FUNCTION_H
#include "libpnm.h"

float babylonian_sqrt(float number);
void calculate_average(const int* info, int length, float *avg_ptr);
void create_histogram_csv(const char* base_filename, unsigned int searching_buffer_size, const char* type, const int* info, int length);
void save_compressed_file(const char* filename, unsigned int searching_buffer_size, int num_tokens, int* offsets, int* lengths, unsigned char* next_symbols, int max_gray_value, int width, int height);
void calculate_std(const int* info, int length, const float* avg_ptr, float* std_ptr);
void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size,
                       float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr);

#endif //LZ77_ENCODING_FUNCTION_H
