

#include "mean_absolute_error.h"
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_pgm_file_name_1>.pgm <input_pgm_file_name_2>.pgm\n", argv[0]);
        return 0;
    }

    char *inputFileName1 = argv[1];
    char *inputFileName2 = argv[2];
    const char *inputExtension = ".pgm";

    // input files must be pgm
    if (strlen(inputFileName1) < strlen(inputExtension) || strcmp(inputFileName1 + strlen(inputFileName1) - strlen(inputExtension), inputExtension) != 0 ||
        strlen(inputFileName2) < strlen(inputExtension) || strcmp(inputFileName2 + strlen(inputFileName2) - strlen(inputExtension), inputExtension) != 0) {
        fprintf(stderr, "Usage: %s <input_pgm_file_name_1>.pgm <input_pgm_file_name_2>.pgm\n", argv[0]);
        return 0;
    }

    float error = mean_absolute_error(inputFileName1, inputFileName2);

    if(error != -1)
        printf("The mean absolute error between files <%s> and <%s> is: %f", inputFileName1, inputFileName2, error);
    else
        printf("Unable to calculate mean absolute error. The two pgm files must have the same dimensions.");

}