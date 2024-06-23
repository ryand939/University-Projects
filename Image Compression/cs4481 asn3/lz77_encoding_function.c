
#include "lz77_encoding_function.h"

// babylonian method to get square root because i dont know if we can include math.h
float babylonian_sqrt(float number) {
    if (number <= 0.0f) return 0.0f;
    float guess = number / 2.0f;
    float previous = 0.0f;
    while (guess != previous) {
        previous = guess;
        guess = (guess + number / guess) / 2.0f;
    }
    return guess;
}

void calculate_average(const int* info, int length, float *avg_ptr) {
    // get sum
    int sum = 0;
    for (int i = 0; i < length; i++)
        sum += info[i];
    // divide by length
    *avg_ptr = (float)sum / length;
}

// creates histogram from array of integers
void create_histogram_csv(const char* base_filename, unsigned int searching_buffer_size, const char* type, const int* info, int length){
    char filename[100];
    // name is: base_filename + . + search_buffer_size + . + type + . + csv
    snprintf(filename, sizeof(filename), "%s.%u.%s.csv", base_filename, searching_buffer_size, type);

    // get max value for memory allocation
    int max_value = 0;
    for (int i = 0; i < length; i++)
        if (info[i] > max_value)
            max_value = info[i];

    int* histogram = calloc(max_value + 1, sizeof(int));
    if (!histogram) {
        printf("allocation for histogram failed.\n");
        return;
    }

    // get frequency of each integer
    for (int i = 0; i < length; i++)
        histogram[info[i]]++;

    // open file or dont
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open histogram csv file");
        free(histogram);
        return;
    }

    // print all the data into the file now
    for (int i = 0; i <= max_value; i++)
        // only print if frequency > 0
        if (histogram[i] > 0)
            // format: value, freq
            fprintf(file, "%d,%d\n", i, histogram[i]);


    // done
    fclose(file);
    free(histogram);
}

// function to save lz compressed file
void save_compressed_file(const char* filename, unsigned int searching_buffer_size, int num_tokens, int* offsets, int* lengths, unsigned char* next_symbols, int max_gray_value, int width, int height){
    char output_filename[100];
    // same input file name + . + search buffer size + .lz
    snprintf(output_filename, sizeof(output_filename), "%s.%u.lz", filename, searching_buffer_size);

    // open file to write all the data
    FILE* file = fopen(output_filename, "wb");
    if (!file) {
        perror("Failed to open output lz file");
        return;
    }
    // write the header
    fwrite(&width, sizeof(int), 1, file);
    fwrite(&height, sizeof(int), 1, file);
    fwrite(&max_gray_value, sizeof(int), 1, file);
    fwrite(&searching_buffer_size, sizeof(unsigned int), 1, file);
    fwrite(&num_tokens, sizeof(int), 1, file);

    // write the token data
    fwrite(offsets, sizeof(int), num_tokens, file);
    fwrite(lengths, sizeof(int), num_tokens, file);
    fwrite(next_symbols, sizeof(unsigned char), num_tokens, file);

    // done
    fclose(file);
}

// function to calculate standard deviation given an array of integers
void calculate_std(const int* info, int length, const float* avg_ptr, float* std_ptr){
    // variance using mean/avg calculated before
    float variance = 0.0f;
    for (int i = 0; i < length; ++i) {
        // (info[i] - mean)^2
        float diff = info[i] - *avg_ptr;
        variance += diff * diff;
    }
    variance /= (float)length;

    *std_ptr = babylonian_sqrt(variance);
}

// function to encode a pgm image using lz77 encoding
void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size,
                       float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr){

    // load image to be encoded
    struct PGM_Image pgm;
    if(load_PGM_Image(&pgm, in_PGM_filename_Ptr) == -1)
        printf("Failed to load PGM!\n");


    // worst case number of tokens is number of total pixels in image
    int num_pixels = pgm.height * pgm.width;

    // LZ77 tokens
    int *offsets = malloc(num_pixels * sizeof(int));                           // array for offsets
    int *lengths = malloc(num_pixels * sizeof(int));                           // array for length of match
    unsigned char *next_symbols = malloc(num_pixels * sizeof(unsigned char));   // array for next symbol
    int num_tokens = 0;

    // logic for variable size search buffer window
    unsigned char search_buffer[searching_buffer_size];
    int search_buffer_start = 0;
    int search_buffer_end = 0;

    // input stream of unsigned chars (pixels of image to encode)
    unsigned char input_stream[num_pixels];

    // scan through all gray values left to right, top to bottom and get the input stream
    int index = 0;
    for (int row = 0; row < pgm.height; row++)
        for (int col = 0; col < pgm.width; col++) {
            input_stream[index] = pgm.image[row][col];
            index++;
        }

    // current length of search buffer
    int buffer_length = 0;
    // best offset, best length, and next symbol found for the current token
    int best_length;
    int best_offset;
    unsigned char next_symbol;

    // loop through the input stream
    for (int x = 0; x < num_pixels; x++) {

        //printf("ON SYMBOL: %d\n", input_stream[x]);
        next_symbol = input_stream[x];
        best_length = 0;
        best_offset = 0;



        // search the buffer to find the longest match
        for (int i = 0; i < buffer_length; i++) {
            int match_length = 0;

            while (i + match_length < buffer_length &&  // while current buffer index + length of current match < buffer length
                   x + match_length < num_pixels &&     // and input stream index + length of current match < length of input stream
                                                        // and the current character in the search buffer == next character in the match
                   search_buffer[(search_buffer_start + i + match_length) % searching_buffer_size] == input_stream[x + match_length]) {
                match_length++;
            }
            // overwrite best match if longer or same size, to get smaller offset
            if (match_length != 0 && match_length >= best_length) {
                best_length = match_length;
                best_offset = buffer_length - i;
            }
        }

        // get the next symbol
        if(x + best_length < num_pixels) {
            // at least one symbol available after the matched sequence
            next_symbol = input_stream[x + best_length];
        } else if(!(x == num_pixels - 1 && best_length == 0)){
            // no symbols after match sequence but best_length>0
            // so decrease the best_length by one, and make the next symbol the last symbol in the input stream
            next_symbol = input_stream[x + best_length - 1];
            best_length--;
        }

        // done creating token, record current token
        offsets[num_tokens] = best_offset;
        lengths[num_tokens] = best_length;
        next_symbols[num_tokens] = next_symbol;
        num_tokens++;

        // add symbol(s) to buffer, upper loop bound best_length + 1 so will always add one symbol even if no match found in buffer
        for (int j = 0; j < best_length + 1; j++) {
            // dont go beyond input stream bounds
            if (x + j < num_pixels) {
                // add symbols to circular buffer
                search_buffer[search_buffer_end] = input_stream[x + j];
                search_buffer_end = (search_buffer_end + 1) % searching_buffer_size;

                // if the buffer is full, increment the "start" index for circular array
                if (buffer_length == searching_buffer_size) {
                    search_buffer_start = (search_buffer_start + 1) % searching_buffer_size;
                } else {
                    // buffer not full, start can stay at 0, but buffer length is increased
                    buffer_length++;
                }
            }
        }
        // skip the next <best_length> number of tokens because they should have just been added to buffer
        x += best_length;
        //printf("TOKEN (%d, %d, %u) %d\n\n", best_offset, best_length, next_symbol, x);
    }

    // save lz compressed file
    save_compressed_file(in_PGM_filename_Ptr, searching_buffer_size, num_tokens, offsets, lengths, next_symbols, pgm.maxGrayValue, pgm.width, pgm.height);

    // histogram for offsets
    create_histogram_csv(in_PGM_filename_Ptr, searching_buffer_size, "offsets", offsets, num_tokens);

    // histogram for lengths
    create_histogram_csv(in_PGM_filename_Ptr, searching_buffer_size, "lengths", lengths, num_tokens);

    // average lengths
    calculate_average(lengths, num_tokens, avg_length_Ptr);

    // average offsets
    calculate_average(offsets, num_tokens, avg_offset_Ptr);

    // std dev for lengths
    calculate_std(lengths, num_tokens, avg_length_Ptr, std_length_Ptr);

    // std dev for offsets
    calculate_std(offsets, num_tokens, avg_offset_Ptr, std_offset_Ptr);

    // done, free memory
    free(offsets);
    free(lengths);
    free(next_symbols);
    free_PGM_Image(&pgm);

}
