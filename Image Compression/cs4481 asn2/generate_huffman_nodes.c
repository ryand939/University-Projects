//
// Created by doubl on 2/25/2024.
//

#include "generate_huffman_nodes.h"


struct node *generate_huffman_nodes(long int *pixel_frequency, int max_gray_value, int number_of_non_zero_values_in_the_frequency_array){
    struct node *nodeArr;
    // account for if there is only one color in the pgm.
    // in this case, the "huffman node" will just be the value of the one color paired with -1 to indicate one color
    if(number_of_non_zero_values_in_the_frequency_array == 1)
        nodeArr = calloc(1, sizeof(struct node));
    else
        nodeArr = calloc((number_of_non_zero_values_in_the_frequency_array - 1), sizeof(struct node));

    if (nodeArr == NULL) {
        fprintf(stderr, "Couldn't allocate memory for node array.\n");
        return NULL;
    }


    for (int i = 0; i < ((number_of_non_zero_values_in_the_frequency_array == 1) ? 1 : number_of_non_zero_values_in_the_frequency_array - 1); i++){
        int index1 = -1, index2 = -1;
        for (int x = 0; x <= max_gray_value; x++) {
            if (pixel_frequency[x] > 0) {
                // check if 1st index empty or if the current pxl is smaller than the current index
                if (index1 == -1 || pixel_frequency[x] < pixel_frequency[index1]) {
                    index2 = index1;
                    index1 = x;
                }
                // check if 2nd index empty or if the current pxl is smaller than the current index
                else if (index2 == -1 || pixel_frequency[x] < pixel_frequency[index2]) {
                    index2 = x;
                }
            }
        }
        // always prefer lower index to store sum, and for proper consistent huffman pairs
        if(index1 > index2 && index2 != -1){
            int temp = index2;
            index2 = index1;
            index1 = temp;
        }
        // store sum of two smallest in the lower index of the two
        if(index2 != -1)
        {
            pixel_frequency[index1] += pixel_frequency[index2];
            // mark pixel 0 to not check again
            pixel_frequency[index2] = 0;
        }
        nodeArr[i].first_value = index1;
        nodeArr[i].second_value = index2;
    }
    return nodeArr;

}