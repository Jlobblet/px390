// Write a function with prototype
// int arr_norm(int *arr, int len)
// which returns the norm of the array arr defined as the sum of the
// square of each of the elements of the array arr, which has length len.

#include <stdio.h>

// begin question

int arr_norm(int* arr, int len) {
    int ret = 0;
    for (int i = 0; i < len; i++) {
        ret += arr[i] * arr[i];
    }
    return ret;
}

// end question

int main() {
#define ARR_LENGTH 3
    int arr[ARR_LENGTH] = {1, 2, 3};
    printf("%i\n", arr_norm(arr, ARR_LENGTH));
    return 0;
}
