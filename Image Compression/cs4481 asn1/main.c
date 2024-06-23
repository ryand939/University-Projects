#include <stdio.h>
#include <stdlib.h>
#include "libpnm.h"
#include <string.h>

#define TRUE 1
#define FALSE 0
#define ASCII 0
#define RAW 1
#define PBM 1
#define PGM 2
#define PPM 3

// struct to represent a single rgb pixel. used for ppm images
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGBPixel;

// Checks for valid filename input, returns 0 if not pgm pbm ppm or otherwise invalid (no filename etc)
int getFileType(const char *filename) {
    const char *pbmExt = ".pbm";
    const char *pgmExt = ".pgm";
    const char *ppmExt = ".ppm";
    size_t filename_len = strlen(filename);

    if (filename_len > strlen(pbmExt) && strcmp(filename + filename_len - strlen(pbmExt), pbmExt) == 0) {
        return PBM;
    } else if (filename_len > strlen(pgmExt) && strcmp(filename + filename_len - strlen(pgmExt), pgmExt) == 0) {
        return PGM;
    } else if (filename_len > strlen(ppmExt) && strcmp(filename + filename_len - strlen(ppmExt), ppmExt) == 0) {
        return PPM;
    }
    // not pbm pgm or ppm
    return 0;
}

// this function determines the denominator for a given decimal
// should be sufficient for the decimal values made by this program
int findDenominator(double decimal) {
    int denominator;
    const double tolerance = 1e-6;
    for (denominator = 1; denominator <= 10000; denominator++) {
        double numerator = (int)(decimal * denominator + 0.5);
        double fraction = numerator / denominator;
        if (decimal > fraction ? decimal - fraction < tolerance : fraction - decimal < tolerance) {
            break;
        }
    }
    return denominator;
}

// helper function to round a number up
double simpleCeil(double num) {
    if (num == (int)num) {
        return num;
    } else if (num > 0) {
        return (int)num + 1;
    } else {
        return (int)num;
    }
}


void addBorder(int height, int width, int arr[height][width], int borderNum)
{
    // set every index to 0
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            arr[i][j] = 0;
    // create borders 25%
    int topBorder = height / 4;
    int sideBorder = width / 4;
    for (int x = 0; x < width; x++)
        for (int y = 0; y < topBorder; y++) {
            arr[y][x] = borderNum;
            arr[height - 1 - y][x] = borderNum;
        }
    for (int x = 0; x < height; x++)
        for (int y = 0; y < sideBorder; y++) {
            arr[x][y] = borderNum;
            arr[x][width - y - 1] = borderNum;
        }
}

void generatePBM(int height, int width, const char *fname, int fcode) {

    int arr[height][width];

    addBorder(height, width, arr, 1);

    // determine the size of white box
    int bHeight = height / 2;
    int bWidth = width / 2;

    double rem;
    int modHeight, modWidth;
    int rotated = FALSE;
    if(bWidth > bHeight)
    {
        rotated = TRUE;
        modHeight = bHeight;
        modWidth = bWidth;

    }else{
        modHeight = bWidth;
        modWidth = bHeight;
    }
    rem = (float)modWidth/(float)modHeight;
    int modArr[modWidth][modHeight];

    for (int i = 0; i < modWidth; i++)
        for (int j = 0; j < modHeight; j++)
            modArr[i][j] = 0;


    //int slope = ceil(rem);
    int slope = (int)simpleCeil(rem);
    int altSlope = (int)rem;
    //forbidden addition
    double dontAdd = rem - (int)rem;
    // this bit of code determines the denominator that corresponds to the <1.0 remainder from the division of height/width
    // this will help determine when to use the alternate slope
    int denominator = findDenominator(dontAdd);
    // counter represents how many 1s were placed in a row/column to not exceed the slope
    // row represents the current row/column we are placing 1s in vertically
    int counter = 0, row = 0, totalCounter = 0;
    for(int x = 0; x < modWidth; x++){
        for(int y = 0; y < modHeight; y++){
            if(y == row){
                if(counter == slope){
                    counter = 0;
                    row++;

                // We must use the alternate slope a certain number of times to make the line even
                // the (- 0.00001) accounts for inconsistent rounding that gave me issues while testing
                }else if((double)((row) % (denominator))/denominator >= dontAdd - 0.00001)
                {
                    modArr[x][y] = 1;
                    modArr[x][modHeight - y - 1] = 1;
                    // add the remaining points for the alternate slope and then go to the next row/column
                    for(int z = 0; z < altSlope; z++){
                        modArr[x + z][y] = 1;
                        modArr[x + z ][modHeight - y - 1] = 1;
                    }
                    x = x + altSlope - 1;
                    row++;
                    break;
                }

                else{
                    totalCounter++;
                    counter++;
                    modArr[x][y] = 1;
                    modArr[x][modHeight - y - 1] = 1;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < bHeight; i++) {
        for (int j = 0; j < bWidth; j++) {

            if(rotated == FALSE){
                arr[i + height / 4][j + width / 4] = modArr[i][j];
            }else{
                arr[i + height / 4][j + width / 4] = modArr[j][i];
              }
        }
    }
    // now making the actual pbm
    FILE *file = fopen(fname, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    if(fcode == ASCII)
        fprintf(file, "P1\n");
    else
        fprintf(file, "P4\n");

    fprintf(file, "%d %d\n", width, height);

    if(fcode == RAW) {
        unsigned char byte = 0; // byte to get added
        int bitIndex = 0; // bit counter
        for (int i = 0; i < height; i++) {
            // new row reset byte and index
            byte = 0;
            bitIndex = 0;
            for (int j = 0; j < width; j++) {
                byte <<= 1; // shift current byte 1 left
                byte |= (arr[i][j] & 1); // add bit pixel, & ensures its only 1 or 0
                bitIndex++;
                if (bitIndex == 8) { // byte ready to be added, reset
                    fwrite(&byte, 1, 1, file);
                    byte = 0;
                    bitIndex = 0;
                }
            }
            // remaining bits for row, case where width mod 8 != 0
            if (bitIndex > 0) {
                byte <<= (8 - bitIndex);
                fwrite(&byte, 1, 1, file);
            }
    }} else { // ASCII much more simple, just copy the array
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                fprintf(file, "%d ", arr[i][j]);
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);

}

void generatePGM(int height, int width, const char *fname, int fcode)
{
    int arr[height][width];


    // add border
    addBorder(height, width, arr, 0);

    // determine the size of white box
    int bHeight = height / 2;
    int bWidth = width / 2;

    int simHeight = bHeight / 2;
    int simWidth = bWidth / 2;

    int simArr[simHeight][simWidth];
    for(int x = 0; x < simHeight; x++)
        for(int y = 0; y < simWidth; y++)
            simArr[x][y] = 255;



    // cut out the top left corner of the work area, get the slope from the top left to bottom right
    // for any given point, determine if the coordinates are above or below the slope,
    // with that info, color the pixel based on its distance to the center of the image
    // when the corners are put back together, you will get the 4 triangles

    double slope = (double)simHeight / simWidth;
    double currPos;
    for(int x = 0; x < simHeight; x++){
        for(int y = 0; y < simWidth; y++)
        {
            currPos = (double)(x+1)/(y+1);
            if(currPos > slope)
                simArr[x][y] = 255 - ((double)y/(simWidth - 1)) * 255;
            // edge case where height = 4, so simHeight - 1 = 0, x = 0, ends up with 0/0 if not caught
            else if(simHeight - 1 == 0)
                simArr[x][y] = 0;
            // height > 4, so no 0/0 issues
            else
                simArr[x][y] =  (255 - (double)x/(simHeight - 1) * 255);


        }
    }
    for(int x = 0; x < simHeight; x++){
        for(int y = 0; y < simWidth; y++)
        {
            // update top left
            arr[simHeight + x][simWidth + y] = simArr[x][y];
            // update bottom left
            arr[simHeight*2 + simHeight - x - 1][simWidth + y] = simArr[x][y];
            // update bottom right
            arr[simHeight*2 + (simHeight - x - 1)][2*simWidth + (simWidth - y - 1)] = simArr[x][y];
            // update top right
            arr[simHeight + x][2*simWidth + simWidth - y - 1] = simArr[x][y];
        }

    }

    // output to pgm file!
    FILE *file = fopen(fname, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    if(fcode == ASCII)
        fprintf(file, "P2\n");
    else
        fprintf(file, "P5\n");

    // width height and max pixel value is 255
    fprintf(file, "%d %d 255\n", width, height);


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if(fcode == RAW) // write bytes for raw format or integers for ascii
            {
                unsigned char value = arr[i][j];
                fwrite(&value, sizeof(unsigned char), 1, file);
            }
            else
                fprintf(file, "%d ", arr[i][j]);
        }
        if(fcode == ASCII)
            fprintf(file, "\n");
    }

    fclose(file);
}

void generatePPM(int height, int width, const char *fname, int fcode)
{
    RGBPixel arr[height][width];

    int thirdWidth = width / 3;
    int halfWidth = width / 2;
    int halfHeight = height / 2;

    // set all values to 0
    for(int x = 0; x < height; x++){
        for(int y = 0; y < width; y++)
        {
            arr[x][y].r = 255;
            arr[x][y].g = 255;
            arr[x][y].b = 255;
        }
    }

    // thirds
    for(int x = 0; x < halfHeight; x++)
    {
        for(int y = 0; y < thirdWidth; y++)
        {
            // red
            arr[x][y].g -= 255 - ((double)x/(halfHeight - 1)) * 255;
            arr[x][y].b -= 255 - ((double)x/(halfHeight - 1)) * 255;
            // green
            arr[halfHeight - x - 1][thirdWidth + y].r -= 255 - ((double)x/(halfHeight - 1)) * 255;
            arr[halfHeight - x - 1][thirdWidth + y].b -= 255 - ((double)x/(halfHeight - 1)) * 255;
            // blue
            arr[x][thirdWidth*2 + y].r -= 255 - ((double)x/(halfHeight - 1)) * 255;
            arr[x][thirdWidth*2 + y].g -= 255 - ((double)x/(halfHeight - 1)) * 255;
        }
    }

    // lower half
    for(int x = 0; x < halfHeight; x++)
    {
        for(int y = 0; y < halfWidth; y++)
        {
            // left half
            arr[x + halfHeight][y].r -= 255 - ((double)x/(halfHeight - 1)) * 255;
            arr[x + halfHeight][y].g -= 255 - ((double)x/(halfHeight - 1)) * 255;
            arr[x + halfHeight][y].b -= 255 - ((double)x/(halfHeight - 1)) * 255;

            // right half
            arr[x + halfHeight][y + halfWidth].r -= ((double)x/(halfHeight - 1)) * 255;
            arr[x + halfHeight][y + halfWidth].g -= ((double)x/(halfHeight - 1)) * 255;
            arr[x + halfHeight][y + halfWidth].b -= ((double)x/(halfHeight - 1)) * 255;
        }
    }


    // output to ppm file

    FILE *file = fopen(fname, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    if(fcode == ASCII)
        fprintf(file, "P3\n");
    else
        fprintf(file, "P6\n");

    // width height and max pixel color value is 255
    fprintf(file, "%d %d 255\n", width, height);


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if(fcode == RAW) // write bytes for raw format or integers for ascii
                fwrite(&arr[i][j], sizeof(RGBPixel), 1, file);
            else
                fprintf(file, "%d %d %d ", arr[i][j].r, arr[i][j].g, arr[i][j].b);
        }
        if(fcode == ASCII)
            fprintf(file, "\n");
    }

    fclose(file);


    /*****
     *
     *  I am only just realizing now that libpnm.h has many functions that could have made
     *  my life much easier while completing this assignment. I knew I had to use it, but
     *  I thought it was only for program-3.
     *
     *******/

    // convert my RGBPixel array to char array
    struct PPM_Image myImg;
    create_PPM_Image(&myImg, width, height, 255);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            myImg.image[i][j][0] = arr[i][j].r;
            myImg.image[i][j][1] = arr[i][j].g;
            myImg.image[i][j][2] = arr[i][j].b;
        }
    }

    struct PGM_Image myPGM;
    create_PGM_Image(&myPGM, width, height, 255);
    char filename[100];

    copy_PPM_to_PGM(&myImg, &myPGM, RED);
    snprintf(filename, sizeof(filename), "red_value_only_%d_%d_%s_ppm_to_pgm.pgm", width, height, fcode == 1 ? "raw" : "ascii");
    save_PGM_Image(&myPGM, filename, fcode);
    copy_PPM_to_PGM(&myImg, &myPGM, GREEN);
    snprintf(filename, sizeof(filename), "green_value_only_%d_%d_%s_ppm_to_pgm.pgm", width, height, fcode == 1 ? "raw" : "ascii");
    save_PGM_Image(&myPGM, filename, fcode);
    copy_PPM_to_PGM(&myImg, &myPGM, BLUE);
    snprintf(filename, sizeof(filename), "blue_value_only_%d_%d_%s_ppm_to_pgm.pgm", width, height, fcode == 1 ? "raw" : "ascii");
    save_PGM_Image(&myPGM, filename, fcode);


    free_PPM_Image(&myImg);
    free_PGM_Image(&myPGM);
}

int main(int argc, char *argv[]) {

    if(argc != 6){
        printf("Please provide 5 arguments:\n1. Type code - 1 for pbm, 2 for pgm, 3 for ppm"
               "\n2. Image width\n3. Image height\n4. Output name including extension\n5. Image format code - 0 for ASCII, 1 for raw\n");
        return 0;
    }
    char *validCheck;
    int typeCode, width, height, formatCode, fileExt;

    // test for valid type code 1-3
    typeCode = strtol(argv[1], &validCheck, 10);
    if(*validCheck || (typeCode < 1 || typeCode > 3)){ printf("Invalid type code.\n"); return 0; }

    // test for valid width, divisible and >=6 if typecode is 3, or divisible and >=4 if typecode is 1 or 2
    width = strtol(argv[2], &validCheck, 10);
    if(*validCheck || !((width % 6 == 0 && width > 5 && typeCode == 3) || (width % 4 == 0 && width > 3 && typeCode != 3))){ printf("Invalid width.\n"); return 0; }

    // test for valid height, divisible by 4 and >=4
    height = strtol(argv[3], &validCheck, 10);
    if(*validCheck ||  !(height % 4 == 0 && height > 3)){ printf("Invalid height.\n"); return 0; }

    // test for valid file name and extension, the validation test cases do not use extension but the assignment outline says to include ext
    fileExt = getFileType(argv[4]);
    if(!((typeCode == PBM && fileExt == PBM) || (typeCode == PGM && fileExt == PGM) || (typeCode == PPM && fileExt == PPM)))
    { printf("Invalid file name. Include the file name and correct extension.\n"); return 0; }

    // test for valid format code 0 or 1
    formatCode = strtol(argv[5], &validCheck, 10);
    if(*validCheck || (formatCode != ASCII && formatCode != RAW)){ printf("Invalid format code.\n"); return 0; }

    if(typeCode == PBM)
        generatePBM(height, width, argv[4], formatCode);
    else if(typeCode == PGM)
        generatePGM(height, width, argv[4], formatCode);
    else // ppm
        generatePPM(height, width, argv[4], formatCode);


    return 0;
}


