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

int S_BOX[8][4][16] = {
    // S1
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    // S2
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    // S3
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    // S4
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    // S5
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    // S6
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    // S7
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    // S8
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
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

// S-box substitution: input: 48bits, output: 32bits
uint32_t sbox_substitute(uint64_t input) {
    uint32_t output = 0;

    for (int i = 0; i < 8; i++) {
        // Extract the i-th 6-bit block from input
        int six_bit_block = (input >> (42 - i * 6)) & 0x3F;

        // get the row and column of S-box array
        int row = ((six_bit_block & 0x20) >> 4) | (six_bit_block & 0x01); // First and last bit
        int column = (six_bit_block >> 1) & 0x0F; // Middle 4 bits

        int sbox_value = S_BOX[i][row][column];

        output |= (sbox_value << (28 - i * 4));
    }
    return output;
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

    uint64_t input = 0x711732E15CF0;  //48 bits input
    uint32_t output = sbox_substitute(input);
    printf("SBOX result: %X\n", output);

    return 0;
}
