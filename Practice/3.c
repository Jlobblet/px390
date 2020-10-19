// Write a function that outputs the words such as "let" "leet" or
// "leeeeet" to standard output, followed by a return character: the
// function takes an integer argument telling it how many copies of
// the letter 'e' to include.
//
// Function prototype is
// void l33t(int number_of_copies_of_the_letter_e);

// start question

#include <stdio.h>
#include <memory.h>

void l33t(int number_of_copies_of_the_letter_e) {
    unsigned char arr[number_of_copies_of_the_letter_e + 4];
    arr[0] = 'l';
    memset(arr + 1, 'e', number_of_copies_of_the_letter_e);
    arr[number_of_copies_of_the_letter_e + 1] = 't';
    arr[number_of_copies_of_the_letter_e + 2] = '\n';
    arr[number_of_copies_of_the_letter_e + 3] = 0;
    printf("%s", arr);
}

// end question

int main() {
    for (int i = 0; i < 100; ++i) {
        l33t(i);
    }
}
