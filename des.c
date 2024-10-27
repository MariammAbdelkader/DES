#include <stdio.h>


int initial_permutation_table[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};


int final_permutation_table[64] = {
    40, 8, 48, 16, 56, 24, 32, 0,
    41, 9, 49, 17, 57, 25, 33, 1,
    42, 10, 50, 18, 58, 26, 34, 2,
    43, 11, 51, 19, 59, 27, 35, 3,
    44, 12, 52, 20, 60, 28, 36, 4,
    45, 13, 53, 21, 61, 29, 37, 5,
    46, 14, 54, 22, 62, 30, 38, 6,
    47, 15, 55, 23, 63, 31, 39, 7
};

void apply_permutation(unsigned int high_in, unsigned int low_in, unsigned int *high_out, unsigned int *low_out, const int *permutation, int num_bits) {
    unsigned int high_result = 0;
    unsigned int low_result = 0;

    for (int i = 0; i < num_bits; i++) {
        int bit = permutation[i] - 1;
        unsigned int extracted_bit;
        if (bit < 32) {
          
            extracted_bit = (low_in >> (31 - bit)) & 0x1;
        } else {
          
            extracted_bit = (high_in >> (63 - bit)) & 0x1;
        }

        // Place extracted bit in correct position in high or low result
        if (i < 32) {
            high_result |= (extracted_bit << (31 - i));
        } else {
            low_result |= (extracted_bit << (63 - i));
        }
    }

    *high_out = high_result;
    *low_out = low_result;
}

void initial_permutation(unsigned char *block) {
    unsigned int high_in = (block[0] << 24) | (block[1] << 16) | (block[2] << 8) | block[3];
    unsigned int low_in = (block[4] << 24) | (block[5] << 16) | (block[6] << 8) | block[7];
    unsigned int high_out, low_out;


    apply_permutation(high_in, low_in, &high_out, &low_out, initial_permutation_table, 64);

    block[0] = (high_out >> 24) & 0xFF;
    block[1] = (high_out >> 16) & 0xFF;
    block[2] = (high_out >> 8) & 0xFF;
    block[3] = high_out & 0xFF;
    block[4] = (low_out >> 24) & 0xFF;
    block[5] = (low_out >> 16) & 0xFF;
    block[6] = (low_out >> 8) & 0xFF;
    block[7] = low_out & 0xFF;

    printf("Output after initial permutation: ");
    for (int i = 0; i < 8; i++) {
        printf("%02X ", block[i]);
    }
    printf("\n");
}

void final_permutation(unsigned char *block) {
    unsigned int high_in = (block[0] << 24) | (block[1] << 16) | (block[2] << 8) | block[3];
    unsigned int low_in = (block[4] << 24) | (block[5] << 16) | (block[6] << 8) | block[7];
    unsigned int high_out, low_out;

    // Apply permutation using the final_permutation_table
    apply_permutation(high_in, low_in, &high_out, &low_out, final_permutation_table, 64);

    // Store the result back in block
    block[0] = (high_out >> 24) & 0xFF;
    block[1] = (high_out >> 16) & 0xFF;
    block[2] = (high_out >> 8) & 0xFF;
    block[3] = high_out & 0xFF;
    block[4] = (low_out >> 24) & 0xFF;
    block[5] = (low_out >> 16) & 0xFF;
    block[6] = (low_out >> 8) & 0xFF;
    block[7] = low_out & 0xFF;

    printf("Output after final permutation: ");
    for (int i = 0; i < 8; i++) {
        printf("%02X ", block[i]);
    }
    printf("\n");
}

int main() {
    unsigned char block[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

    printf("Original block: ");
    for (int i = 0; i < 8; i++) {
        printf("%02X ", block[i]);
    }
    printf("\n");

    initial_permutation(block);

  
    final_permutation(block);

    return 0;
}
