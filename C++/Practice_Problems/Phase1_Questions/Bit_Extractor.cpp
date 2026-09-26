// Question 1: Bit Extractor and Modulo Math

// You are given a 32-bit unsigned int N. Your task is to extract the middle 16 bits, i.e., bits 8 through 23.

// Then, multiply the extracted 16-bit value by a constant M = 10^9 + 7 and take the modulo with the same M.

// Challenge: Write the solution as a single mathematical/bitwise expression without using any extra line. 
// You must use hexadecimal literals for bitmasking, such as 0xFF.

// Input:

// N = 0x12345678

// Output:

// Extracted value = 0x3456
// Result = 13398

// C Program to Extract multiple bits of a binary number
#include <stdio.h>
#include <cstdio>
#include <cstdint>

const int MOD = 1e9 + 7;

int main()
{

    // Binary: 01100111
    uint32_t num = 0x12345678;

  	// defining the range
    unsigned int start = 8, end = 23;

    // Create a mask
    uint32_t mask = 0;
    for (int i = start; i <= end; i++) {
        mask = mask | (1U << i);
    }

    // Extract the bits using AND and right shift
    unsigned int extracted_bits = (num & mask) >> start;

    unsigned int result = extracted_bits * MOD;

    unsigned int ans = result % MOD; 

    // Print the result
    printf("Extracted Bits: %X\n", extracted_bits);
    printf("Multiplied with MOD = 1e10 + 7: %U\n", result);
    printf("Then Modulus with MOD: %U\n", ans);

    return 0;
}