
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "lz77_encoding_function.h"

int main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_pgm_name> <searching_buffer_size>\n", argv[0]);
        return 0;
    }

    // Check that the file name ends with .pgm
    char *filename = argv[1];
    const char *pgm_extension = ".pgm";
    if (strlen(filename) <= strlen(pgm_extension) ||
        strcmp(filename + strlen(filename) - strlen(pgm_extension), pgm_extension) != 0) {
        fprintf(stderr, "Enter a valid pgm file!\n");
        return 0;
    }

    // check if 2nd arg (buffer size) is an unsigned int, not string
    char *check_null_term;
    unsigned int searching_buffer_size = strtol(argv[2], &check_null_term, 10);
    if (*check_null_term != '\0') {
        fprintf(stderr, "Search buffer must be a valid unsigned int!\n");
        return 0;
    }

    // for measuring time, start "start" now
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // encode pgm
    float avg_offset, std_offset, avg_length, std_length;
    Encode_Using_LZ77(filename, searching_buffer_size, &avg_offset, &std_offset, &avg_length, &std_length);

    // get end time after encoding
    gettimeofday(&end, NULL);
    // elapsed time is difference between start time and end time
    long seconds = (end.tv_sec - start.tv_sec);
    long total_micros = (seconds * 1000000) + end.tv_usec - start.tv_usec;
    seconds = total_micros / 1000000;
    // 1 000 000 microseconds in second, get remaining microseconds
    long micros = total_micros % 1000000;

    // results of encoding
    printf("Encoding Complete For File %s\n", filename);
    printf("Average Offset: %.2f\n", avg_offset);
    printf("Average Length: %.2f\n", avg_length);
    printf("Standard Deviation of Offsets: %.2f\n", std_offset);
    printf("Standard Deviation of Lengths: %.2f\n", std_length);
    printf("Encode Time: %ld seconds and %ld microseconds\n", seconds, micros);

    return 0;
}
