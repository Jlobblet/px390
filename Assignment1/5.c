// Write a PROGRAM (ie with a main function) to read two long integers (A and B) from standard input
// and output (to standard output) A to the power of B.

// This is the same as practice question 1

#include <stdio.h>


// Raise base to the power of exponent.
// This function takes advantage of the fact that exponents
// can be separated and multiplied together.
// For example, b^7 = b * b^2 * b^4
long long long_pow(long long base, long long exponent) {
    long long r = 1;
    while (exponent) {
        if (exponent & 1) {
            r *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    return r;
}

int main() {
    long long A;
    scanf("%lld", &A);
    long long B;
    scanf("%lld", &B);
    long long result;
    result = long_pow(A, B);
    printf("%lld", result);
    return 0;
}

