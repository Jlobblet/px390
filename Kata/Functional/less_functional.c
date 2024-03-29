#include <stdlib.h>
#include <stdio.h>
#include "functions.h"

// Mutate the input array by applying the given function to each element in the input array
void map(int* array, int length, int (* mapping)(int)) {
    for (int i = 0; i < length; i++) {
        array[i] = mapping(array[i]);
    }
}

// Mutate the input array by applying the given function to each element in the input array.
// The first integer passed to the function is the index of the element.
void mapi(int* array, int length, int (* mapping)(int, int)) {
    for (int i = 0; i < length; i++) {
        array[i] = mapping(i, array[i]);
    }
}

// Perform an action, with each element of the array passed to it as parameter.
void apply(int* array, int length, void (* action)(int)) {
    for (int i = 0; i < length; i++) {
        action(array[i]);
    }
}

// Apply a function to each element in the collection, threading an accumulator argument through the
// computation. Take the second argument, then apply the function to it and the first element of the list.
// Then feed this result into the function along with the second element, and so on. Return the final result.
// If the input function is f and the elements are i0...iN, then compute f(...f(initVal, i0), i1)..., iN)
int fold(int* array, int length, int (* folder)(int, int), int initVal) {
    int acc = initVal;
    for (int i = 0; i < length; i++) {
        acc = folder(acc, array[i]);
    }
    return acc;
}

void scan(int* array, int length, int (* folder)(int, int), int initVal) {
    int acc = initVal;
    for (int i = 0; i < length; i++) {
        acc = folder(acc, array[i]);
        array[i] = acc;
    }
}

void scanr(int* array, int length, int (* folder)(int, int), int initVal) {
    int acc = initVal;
    for (int i = length - 1; i >= 0; i--) {
        acc = folder(acc, array[i]);
        array[i] = acc;
    }
}

int reduce(int* array, int length, int (* folder)(int, int)) {
    int head = array[0];
    int tail[length - 1];
    for (int i = 0; i < length; i++) {
        tail[i] = array[i + 1];
    }
    return fold(tail, length - 1, folder, head);
}

void filter(int** array, int* length, int (* predicate)(int)) {
    int predLen = 0;
    for (int i = 0; i < *length; i++) {
        if (!predicate((*array)[i])) continue;
        (*array)[predLen++] = (*array)[i];
    }
    *array = realloc(*array, sizeof(int) * predLen);
    *length = predLen;
}

void print_arr(int* arr, int len) {
    apply(arr, len, prINT);
    printf("\n");
}

int main() {
    int len = 5;
    int* arr = malloc(sizeof(int) * len);
    for (int i = 0; i < len; i++) {
        arr[i] = i;
    }
    printf("Initial array : ");
    print_arr(arr, len);

    map(arr, len, square);
    printf("Mapped array  : ");
    print_arr(arr, len);

    filter(&arr, &len, isOdd);
    printf("Filtered array: ");
    print_arr(arr, len);

#define REDUCE
#ifdef SCAN
    scan(arr, len, add, 0);
    printf("Scanned array : ");
    print_arr(arr, len);
#endif
#ifdef FOLD
    int folded = fold(arr, len, mult, 1);
    printf("Folded array  : ");
    printf("%i\n", folded);
#endif
#ifdef REDUCE
    int reduced = reduce(arr, len, add);
    printf("Reduced array : ");
    printf("%i\n", reduced);
#endif
    return 0;
}
