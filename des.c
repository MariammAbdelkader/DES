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
    40,	8,	48,	16,	56,	24,	64,	32,
    39,	7,	47,	15,	55,	23,	63,	31,
    38,	6,	46,	14,	54,	22,	62,	30,
    37,	5,	45,	13,	53,	21,	61,	29,
    36,	4,	44,	12,	52,	20,	60,	28,
    35,	3,	43,	11,	51,	19,	59,	27,
    34,	2,	42,	10,	50,	18,	58,	26,
    33,	1,	41,	9,	49,	17,	57,	25

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


static const int expansion_table[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

static const int permutation_table[32] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};


// Permuted Choice 1 function
unsigned long long permuted_choice_1(unsigned long long key) {
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

    unsigned long long sub_key = 0;
    for (int i = 0; i < 56; i++) {
        int bit_position = 64 - pc1_table[i];
        unsigned long long bit = (key >> bit_position) & 1;
        sub_key |= (bit << (55 - i));
    }
    return sub_key;
}

int shift_schedule[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

unsigned int left_circular_shift(unsigned int half, int shifts) {
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
unsigned long long permuted_choice_2(unsigned long long CD) {
    unsigned long long round_key = 0;
    for (int i = 0; i < 48; i++) {
        int bit_position = 56 - pc2_table[i];
        unsigned long long bit = (CD >> bit_position) & 1;
        round_key |= (bit << (47 - i));
    }
    return round_key;
}

// Function to print the round key in the specified format
void print_round_key(unsigned long long round_key, int round) {
    printf("K%d = ", round);
    for (int i = 47; i >= 0; i--) {
        printf("%lld", (round_key >> i) & 1);
        if (i % 6 == 0 && i != 0) printf(" ");
    }
    printf("\n");
}
unsigned long long round_keys[16] = {0};
// Generate round keys with binary output for each round
void generate_round_keys(unsigned long long key) {
    unsigned long long subkey = permuted_choice_1(key);
    unsigned int L = (subkey >> 28) & 0x0FFFFFFF;
    unsigned int R = subkey & 0x0FFFFFFF;

    for (int round = 0; round < 16; round++) {
        L = left_circular_shift(L, shift_schedule[round]);
        R = left_circular_shift(R, shift_schedule[round]);
        unsigned long long LR = ((unsigned long long)L << 28) | R;
        round_keys[round] = permuted_choice_2(LR);
        print_round_key((unsigned long long)round_keys[round], round + 1);
    }
}

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
unsigned long long initial_permutation(unsigned long long message) {
    unsigned long long permuted_message = 0;

    for (int i = 0; i < 64; i++) {
        int bit_position = 64 - ip_table[i];
        unsigned long long bit = (message >> bit_position) & 1;
        permuted_message |= (bit << (63 - i));
    }

    return permuted_message;
}
// Function to print a 64-bit integer in binary format, grouped by 8 bits
void print_64bit_binary(unsigned long long value) {
    for (int i = 63; i >= 0; i--) {
        printf("%lld", (value >> i) & 1);
        if (i % 8 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}

unsigned long long final_permutation(unsigned long long message) {
    unsigned long long permuted_message = 0;

    for (int i = 0; i < 64; i++) {
        int bit_position = 64 - final_permutation_table[i];
        unsigned long long bit = (message >> bit_position) & 1;
        permuted_message |= (bit << (63 - i));
    }

    return permuted_message;
}

// S-box substitution: input: 48bits, output: 32bits
unsigned int sbox_substitute(unsigned long long input) {
    unsigned int output = 0;

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

unsigned int permute(unsigned int data) {
    unsigned int result = 0;
    for (int i = 0; i < 32; i++) {
        result |= ((data >> (32 - permutation_table[i])) & 0x1) << (31 - i);
    }
    return result;
}

unsigned long long expand(unsigned int data) {
    unsigned long long result = 0;
    for (int i = 0; i < 48; i++) {
        result |= ((unsigned long long)(data >> (32 - expansion_table[i])) & 0x1) << (47 - i);
    }
    return result;
}

void process_block(unsigned long long *data,  int encrypt) {

    unsigned int L = (*data >> 32) & 0xFFFFFFFF;
    unsigned int R = *data & 0xFFFFFFFF;
    for (int round = 0; round < 16; round++) {
        unsigned int temp = R;
        
        unsigned long long expanded = expand(R);
        printf("round %d: after expansion: ",round);
        print_64bit_binary(expanded);
        printf("\n");
        // use keys in reverse order for decryption
        
        expanded ^= round_keys[encrypt ? round : 15 - round];
        printf("round %d: after XOR: ",round);
        print_64bit_binary(expanded);
        printf("\n");
        // S-box
        unsigned int substituted = sbox_substitute(expanded);
        printf("round %d: after sbox: %x \n",round, substituted);
        // Permutation
        unsigned int permuted = permute(substituted);
        printf("round %d: after p: %x \n",round, permuted);
        R = L ^ permuted;
        L = temp;
        printf("Round %d R: %x, L: %x \n",round+1, R, L);
    }
    
    // Final swap
    *data = ((unsigned long long)R << 32) | L;
}

void des_encrypt(unsigned long long *data, unsigned long long key) {
    //unsigned long long round_keys[16];
    generate_round_keys(key);
    process_block(data, 1);  // 1 for encryption
}

void des_decrypt(unsigned long long *data, unsigned long long key) {
    generate_round_keys(key);
    process_block(data, 0);  // 0 for decryption
}

unsigned long long hex_to_bin(char hex) {
    int value = (hex >= '0' && hex <= '9') ? hex - '0' : hex - 'A' + 10;
    return value;
}

int main() {
    unsigned char block[8];
    unsigned long long key = 0x0123456789ABCDEF;
    unsigned long long data = 0x56CC09E7CFDC4CEF;

    int encrypt = 0;


    data = initial_permutation(data);
    print_64bit_binary(data);

    if (encrypt)
        des_encrypt(&data, key);
    else des_decrypt(&data, key);

    for (int i = 0; i < 8; i++) {
        block[7 - i] = (data >> (i * 8)) & 0xFF;
    }

    printf("after encryption: ");
    for (int i = 0; i < 8; i++) {
        printf("%02X ", block[i]);
    }
    printf("\n");

    data = final_permutation(data);

    printf("final result: ");
    print_64bit_binary(data);
    printf("\n");

    // uint64_t input = 0x711732E15CF0;  //48 bits input
    // uint32_t output = sbox_substitute(input);
    // printf("SBOX result: %X\n", output);

  

    return 0;
}
