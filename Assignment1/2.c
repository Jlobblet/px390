// Write a program that reads a real number 'a' from standard input and outputs the integer value
// floor(a) to standard output  (i.e., the number rounded toward negative infinity).
// The output should be formatted as an integer (i.e, without trailing decimals)
//
// Make sure your program works for negative numbers.
//
// (note that you may want to use functions from the math library,
// in which case you will need an appropriate 'include' line)

#include <math.h>
#include <stdio.h>

int main() {
    double a;
    scanf("%lf", &a);
    long long f = (long long)floor(a);
    printf("%lld", f);
    return 0;
}
