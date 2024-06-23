
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "lz77_decoding_function.h"

int main(int argc, char *argv[]) {
    // must be 1 arg
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_lz_name>\n", argv[0]);
        return 0;
    }

    // file must end in .lz
    char *filename = argv[1];
    const char *lz_extension = ".lz";
    if (strlen(filename) <= strlen(lz_extension) ||
        strcmp(filename + strlen(filename) - strlen(lz_extension), lz_extension) != 0) {
        fprintf(stderr, "Enter a valid lz file!\n");
        return 0;
    }


    // for measuring time, start "start" now
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // decode lz file
    Decode_Using_LZ77(filename);
    // get end time after encoding
    gettimeofday(&end, NULL);
    // elapsed time is difference between start time and end time
    long seconds = (end.tv_sec - start.tv_sec);

    long total_micros = (seconds * 1000000) + end.tv_usec - start.tv_usec;
    seconds = total_micros / 1000000;
    // 1 000 000 microseconds in second, get remaining microseconds
    long micros = total_micros % 1000000;

    // results of encoding
    printf("Decoding Complete For File %s\n", filename);

    printf("Decode Time: %ld seconds and %ld microseconds\n", seconds, micros);

    return 0;
}
