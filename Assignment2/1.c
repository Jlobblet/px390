// Write a function to return the array which has as first element, the last element of
// the input array. The remainder of the return array should then contain the rest of the
// input array. The input array should not be modified (or freed). I'm not going to test
// this with zero array length.
//
// The input is an integer array 'arr' of length 'arr_length'. For example,
// given arr = [3, 4, 1, -6], it should return [-6, 3, 4, 1].
//
// int* move_to_front(int *arr, int arr_length);

#include <stdio.h>

// begin question

#include <stdlib.h>

int* move_to_front(const int* arr, int arr_length) {
    int* returnArr = malloc(arr_length * sizeof(int));
    returnArr[0] = arr[arr_length - 1];
    for (int i = 1; i < arr_length; i++) {
        returnArr[i] = arr[i - 1];
    }
    return returnArr;
}

// end question

int main() {
#define ARR_LEN 4
    int arr[ARR_LEN] = {3, 4, 1, -6};
    int* arr2 = move_to_front(arr, ARR_LEN);
    for (int i = 0; i < ARR_LEN; i++) {
        printf("%i ", arr2[i]);
    }
    return 0;
}
