// Write a function with prototype
//
// int isprime(long k);
//
// Use a loop to determine whether a number is prime by checking if it is divisible
// by another natural number above 1. The code should be a function, returning true
// (i.e, non-zero) if prime, false (zero) if non-prime (you may assume the input is a natural number).
//
// I'm only going to be testing with smallish primes so don't worry too much about efficiency.

#include <stdio.h>

// start question

#include <math.h>

// Test whether k is a prime number or not
// If k is prime, return 1
// If k is not prime, return 0
int isprime(long k) {
    if (k == 1) {
        return 0;
    }
    long max = (long)ceil(sqrt((double)k));
    for (long i = 2; i <= max; i++) {
        if (!(k % i)) {
            return 0;
        }
    }
    return 1;
}

// end question

int main() {
    for (long k = 1; k < 100; k++) {
        printf("%ld: ", k);
        printf("%i\n", isprime(k));
    }
}
