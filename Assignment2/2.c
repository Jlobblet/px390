// Write a function to return the array which has elements appearing in reverse order to an
// input integer array 'arr' of length 'arr_length'. For example, given arr = [3, 4, -6],
// it should return an array [-6, 4, 3]. The input array should not be modified.
//
//int  *reverse_order(int *arr, int arr_length);

#define ARR_LEN 3

#include <stdio.h>

// begin question

#include <stdlib.h>

int* reverse_order(int* arr, int arr_length) {
    int* returnArr = malloc(arr_length * sizeof(int));
    for (int i = 0; i < arr_length; i++) {
        returnArr[arr_length - i - 1] = arr[i];
    }
    return returnArr;
}

// end question

int main() {
    int arr[ARR_LEN] = {3, 4, -6};
    int* arr2 = reverse_order(arr, ARR_LEN);
    for (int i = 0; i < ARR_LEN; i++) {
        printf("%i ", arr2[i]);
    }
    return 0;
}
