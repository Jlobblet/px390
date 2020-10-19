// Write a function with prototype:
//
// int polygon_tesselates(int numsides);
// It should returns true (ie 1) if an n-sided polygon can tesselate the plane
// (it should only return one for n=3,4 or 6), and otherwise returns false.
//
//
// Note the error in the original specification! bool is not defined in basic C syntax, so
// an int type should be used.
#include <stdio.h>

// start question

int polygon_tesselates(int numsides) {
    if (numsides == 3 || numsides == 4 || numsides == 6) {
        return 1;
    }
    return 0;
}

// end question

int main() {
    for (int i = 0; i < 10; i++) {
        printf("%i sides: %i\n", i, polygon_tesselates(i));
    }
}