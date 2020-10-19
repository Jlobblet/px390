// Write a PROGRAM (ie with a main function) to read two long integers (A and B)
// from standard input and output (to standard output) A to the power of B.
#include <stdio.h>

long long long_pow(long long base, long long exponent) {
    long long r = 1;
    while (exponent) {
        if (exponent & 1) {
            r *= base;
        }
        exponent >>= 1;
        base *= base;
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
