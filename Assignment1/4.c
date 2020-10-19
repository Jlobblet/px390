// An automated function is required to indicate how happy you are about your assignment,
// by printing a number of happy emoji symbol :-) separated by a single space . A print function should be used to
// print the line
// "I am happy about this assignment :-)"
// but the number of emoji symbols may be increased by setting the argument
// to the function to a number between 0 and 10 (inclusive).
//
// The function prototype is
// void happy_meter(int size);
//
// As an example:
// happy_meter(3)
// would print:
//
// I am happy about this assignment :-) :-) :-)
//
// Pay attention when copying and pasting to match the upper/lower case and punctuation as Moodle is completely pedantic.
// Trailing whitespace will, however, be ignored.

// start question

#include <stdio.h>

void happy_meter(int size) {
    printf("I am happy about this assignment");
    for (int i = 0; i < size; i++) {
        printf(" :-)");
    }
    printf("\n");
}

// end question

int main() {
    for (int i = 0; i < 10; i++) {
        happy_meter(i);
    }
}