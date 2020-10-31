#include <stdio.h>
#include "functions.h"

int square(int n) {
    return n * n;
}

int add(int a, int b) {
    return a + b;
}

int mult(int a, int b) {
    return a * b;
}

int isEven(int n) {
    return (n % 2) == 0;
}

int isOdd(int n) {
    return !isEven(n);
}

void prINT(int n) {
    printf("%i ", n);
}
