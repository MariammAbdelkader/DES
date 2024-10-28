#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// PC-1
static const int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9, 1,
    58, 50, 42, 34, 26, 18, 10, 2,
    59, 51, 43, 35, 27, 19, 11, 3,
    60, 52, 44, 36, 63, 55, 47, 39,
    31, 23, 15, 7, 62, 54, 46, 38,
    30, 22, 14, 6, 61, 53, 45, 37,
    29, 21, 13, 5, 28, 20, 12, 4
};
// Key shifts for each round
static const int shifts_per_round[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};
// PC-2-box
static const int PC2[48] = {
    14, 17, 11, 24, 1, 5, 3, 28,
    15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32
};

// Circular Left shift function for 28-bit halves
int left_shift_28bit(unsigned int half, int shifts) {
    return ((half << shifts) | (half >> (28 - shifts))) & 0x0FFFFFFF;
}
// Get bit from key at a specific position in a 64-bit key
int get_bit_from_key(long long key, int pos) {
    return (key >> (63 - pos)) & 1;
}
// Function to generate round keys
void generate_round_keys(long long key, unsigned char round_keys[16][6]) {
    unsigned int C = 0, D = 0;
    // Apply PC1 to generate the 56-bit permuted key and split into C and D
    for (int i = 0; i < 28; i++) {
        C |= get_bit_from_key(key, PC1[i] - 1) << (27 - i);
        D |= get_bit_from_key(key, PC1[i + 28] - 1) << (27 - i);
    }
    // Generate 16 round keys
    for (int round = 0; round < 16; round++) {
        C = left_shift_28bit(C, shifts_per_round[round]);
        D = left_shift_28bit(D, shifts_per_round[round]);
        unsigned long long combined = ((unsigned long long)C << 28) | D;
        // Apply PC2 to generate each 48-bit round key
        for (int i = 0; i < 48; i++) {
            int pos = PC2[i] - 1;
            round_keys[round][i / 8] |= ((combined >> (55 - pos)) & 1) << (7 - (i % 8));
        }
    }
}

int main(){
   // key example 
    // Define the key as a 64-bit long long integer
    long long key1 = 0x0123456789ABCDEF;
    unsigned char round_keys[16][6] = {0};
    // Generate and print round keys
    generate_round_keys(key1, round_keys);
    for (int i = 0; i < 16; i++) {
        printf("Round %2d key: ", i + 1);
        for (int j = 0; j < 6; j++) {
            printf("%02X ", round_keys[i][j]);
        }
        printf("\n");
    } 
}