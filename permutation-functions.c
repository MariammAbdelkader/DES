/*
 * key-creation.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Mariz
 */

#include <stdio.h>
#include <stdint.h>

// Convert hex character to 4-bit binary representation
uint64_t hex_to_bin(char hex) {
    int value = (hex >= '0' && hex <= '9') ? hex - '0' : hex - 'A' + 10;
    return value;
}

// Permuted Choice 1 function
uint64_t permuted_choice_1(uint64_t key) {
    int pc1_table[56] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
    };

    uint64_t sub_key = 0;
    for (int i = 0; i < 56; i++) {
        int bit_position = 64 - pc1_table[i];
        uint64_t bit = (key >> bit_position) & 1;
        sub_key |= (bit << (55 - i));
    }
    return sub_key;
}

int shift_schedule[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

uint32_t left_circular_shift(uint32_t half, int shifts) {
    return ((half << shifts) | (half >> (28 - shifts))) & 0x0FFFFFFF;
}

int pc2_table[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// Function to apply PC-2 and generate a 48-bit round key from CD
uint64_t permuted_choice_2(uint64_t CD) {
    uint64_t round_key = 0;
    for (int i = 0; i < 48; i++) {
        int bit_position = 56 - pc2_table[i];
        uint64_t bit = (CD >> bit_position) & 1;
        round_key |= (bit << (47 - i));
    }
    return round_key;
}

// Function to print the round key in the specified format
void print_round_key(uint64_t round_key, int round) {
    printf("K%d = ", round);
    for (int i = 47; i >= 0; i--) {
        printf("%lld", (round_key >> i) & 1);
        if (i % 6 == 0 && i != 0) printf(" ");
    }
    printf("\n");
}

// Generate round keys with binary output for each round
void generate_round_keys(uint64_t key) {
    uint64_t subkey = permuted_choice_1(key);
    uint32_t L = (subkey >> 28) & 0x0FFFFFFF;
    uint32_t R = subkey & 0x0FFFFFFF;

    for (int round = 0; round < 16; round++) {
        L = left_circular_shift(L, shift_schedule[round]);
        R = left_circular_shift(R, shift_schedule[round]);
        uint64_t LR = ((uint64_t)L << 28) | R;
        uint64_t round_key = permuted_choice_2(LR);
        //print_round_key(round_key, round + 1);
    }
}

// Initial Permutation table
int ip_table[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

// Function to perform Initial Permutation (IP)
uint64_t initial_permutation(uint64_t message) {
    uint64_t permuted_message = 0;

    for (int i = 0; i < 64; i++) {
        int bit_position = 64 - ip_table[i];
        uint64_t bit = (message >> bit_position) & 1;
        permuted_message |= (bit << (63 - i));
    }

    return permuted_message;
}
// Function to print a 64-bit integer in binary format, grouped by 8 bits
void print_64bit_binary(uint64_t value) {
    for (int i = 63; i >= 0; i--) {
        printf("%lld", (value >> i) & 1);
        if (i % 8 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}
/*
int main() {
    uint64_t key = 0x0123456789ABCDEF;
    uint64_t message = 0x0123456789ABCDEF;
    printf("Generating Round Keys:\n");
    generate_round_keys(key);
    printf("Original Message: 0x%016llX\n", message);
	uint64_t permuted_message = initial_permutation(message);
	printf("Permuted Message: 0x%016llX\n", permuted_message);
	printf("Permuted Message in Binary: ");
	print_64bit_binary(permuted_message);

return 0;
}
*/
