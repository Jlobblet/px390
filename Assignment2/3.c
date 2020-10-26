// Write a function with prototype
// void varr_swap(char *arr, int a, int b)
// which swaps the values at element a of the input array with that at element b.
// (note the indexing uses C notation so first element is element 0).

#define ARR_LENGTH 5

#include <stdio.h>

// begin question

void varr_swap(char *arr, int a, int b) {
    char temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

// end question

void print_array(char *arr, int length) {
    printf("%c", arr[0]);
    for (int i = 1; i < length; i++) {
        printf(" %c", arr[i]);
    }
    printf("\n");
}

int main() {
    char arr[ARR_LENGTH] = {'0', '1', '2', '3', '4'};
    print_array(arr, ARR_LENGTH);
    varr_swap(arr, 1, 3);
    print_array(arr, ARR_LENGTH);
    return 0;
}