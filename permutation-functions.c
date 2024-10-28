/*
 * permutation-functions.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Mariz
 */
#include <stdio.h>
#include <stdint.h>

#define NUM_ROUNDS 16
#define ROUND_KEY_BITS 48

// Convert hex character to 4-bit binary representation
uint64_t hex_to_bin(char hex){
    int value = (hex >= '0' && hex <= '9') ? hex - '0' : hex-'A' + 10;
    return value;
}

// Permuted Choice 1 function
uint64_t permuted_choice_1(uint64_t key){
    // PC-1 table
    int pc1_table[56] = {
        57, 49, 41, 33, 25, 17,  9,
         1, 58, 50, 42, 34, 26, 18,
        10,  2, 59, 51, 43, 35, 27,
        19, 11,  3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
         7, 62, 54, 46, 38, 30, 22,
        14,  6, 61, 53, 45, 37, 29,
        21, 13,  5, 28, 20, 12,  4
    };

    uint64_t sub_key = 0;  //56-bit

    for (int i = 0; i < 56; i++) {
        int bit_position = 64 - pc1_table[i];
        uint64_t bit = (key >> bit_position) & 1;
        sub_key |= (bit << (55 - i));
    }

    // Print the 56-bit subkey
    printf("Subkey (56-bit): ");
    for (int i=55; i>=0; i--) {
        printf("%lld", (sub_key >> i) & 1);
        if (i % 7 == 0) printf(" ");  // Formatting for readability
    }
    printf("\n");

    return sub_key;
}

//Shift schedule for DES key generation
int shift_schedule[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

//Function to perform left circular shift
uint32_t left_circular_shift(uint32_t half, int shifts) {
    return ((half << shifts) | (half >> (28-shifts))) & 0x0FFFFFFF;  //Mask to keep it 28-bit
}

//Function to print a 64-bit value in binary
void print_64bit_binary(uint64_t value) {
    for (int i=63; i>=0; i--) {
        printf("%lld", (value >> i) & 1);
        if (i%8==0) printf(" ");
    }
    printf("\n");
}

// PC-2 table
int pc2_table[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 50, 29, 40, 51, 36,
    61, 34, 47, 55, 30, 39,
    38, 44, 48, 43, 36, 15
};

//Function to apply PC-2 and generate a 48-bit round key
uint64_t permuted_choice_2(uint64_t CD) {
    uint64_t round_key = 0; //48-bit

    // Apply the PC-2 permutation
    for (int i=0; i<48; i++) {
        int bit_position = 56 - pc2_table[i];
        uint64_t bit = (CD >> bit_position) & 1;
        round_key |= (bit << (47 - i));
    }

    return round_key; //48-bit
}

// Function to generate round keys and return them as a fixed-size array
void generate_round_keys(uint64_t key, uint64_t round_keys[NUM_ROUNDS]) {
    uint64_t subkey = permuted_choice_1(key);

    uint32_t L = (subkey >> 28) & 0x0FFFFFFF;  // Left half
    uint32_t R = subkey & 0x0FFFFFFF;          // Right half

    for (int round=0; round<NUM_ROUNDS; round++) {
        // Apply left shifts to both halves according to the shift schedule
        L = left_circular_shift(L, shift_schedule[round]);
        R = left_circular_shift(R, shift_schedule[round]);

        // Concatenate L and R
        uint64_t LR = ((uint64_t)L << 28) | R;

        // Generate the round key using PC-2
        round_keys[round] = permuted_choice_2(LR);
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

//Function to perform Initial Permutation (IP)
uint64_t initial_permutation(uint64_t message) {
    uint64_t permuted_message = 0;

    //Apply the initial permutation using the IP table
    for (int i=0; i<64; i++) {
        int bit_position = 64 - ip_table[i];
        uint64_t bit = (message >> bit_position) & 1;
        permuted_message |= (bit << (63-i));
    }

    return permuted_message;
}

/*
int main() {
    uint64_t key = 0x0123456789ABCDEF;
    uint64_t message = 0x0123456789ABCDEF;
    printf("Original Message: 0x%016llX\n", message);

    // Apply the initial permutation
    uint64_t permuted_message = initial_permutation(message);

    // Print the permuted message
    printf("Permuted Message: 0x%016llX\n", permuted_message);
    printf("Permuted Message in Binary: ");
    print_64bit_binary(permuted_message);

    // Declare an array for round keys
    uint64_t round_keys[NUM_ROUNDS];

    // Generate round keys
    generate_round_keys(key, round_keys);

    return 0;
}
*/


