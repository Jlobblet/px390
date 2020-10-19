// Write a function with prototype:
//
// void decompose(double k, int *S, int *Q, double *F);
//
// The function should not print anything to stdout.
//
// The function arguments are a double precision number k,
// two integer pointers S and Q, and a double precision pointer F.
// The pointers S, Q and F refer to variables s, q, and f.
// Using pointers this way allow a function to provide multiple 'outputs' to the calling function.
//
// On exit from the function, s should be the sign of k, plus or minus 1,
// q should be the largest integer such that 2^q <= |k|,
// and f (usually called the 'fraction') is set such that |k| = f 2^q.
// Note f should be in the range 1 <= f < 2.
//
// You may assume the input k is non-zero and finite.
//
// Hint: there is a standard library function, frexp, that does something similar.

#include <stdio.h>

// start question

#include <math.h>

// this should really return a struct
void decompose(double k, int* S, int* Q, double* F) {
    // branchless programming ftw
    *S = (k > 0) - (k < 0);

    // Q is basically just the position of the leading bit in k
    *Q = 0;
    unsigned long k2 = floor(fabs(k));
    while (k2 >>= 1u) {
        ++*Q;
    }

    // this is just from the spec
    *F = fabs(k) / pow(2.0, *Q);
}

// end question

int main() {
    double k, f;
    int s, q;
    scanf("%lf", &k);
    decompose(k, &s, &q, &f);
    printf("s: %i\nq: %i\nf: %lf\n", s, q, f);
    return 0;
}
