#include <stdio.h>

struct int_array {
    int* arr;
    int len;
}typedef int_array;

void fibonacci (int_array* out) {
    out->arr[0] = 0;
    out->arr[1] = 1;

    for (int i = 2; i <= out->len; i++) {
        out->arr[i] = out->arr[i-1] + out->arr[i-2];
    }
}

int main () {
    int len = 50;
    int arr[len];
    int_array f = {arr, len};
    fibonacci(&f);
    for (int i = 0; i < len; i++){
        printf("Fibonacci number %i: %i\n", i, f.arr[i]);
    }
    return 0;
}
