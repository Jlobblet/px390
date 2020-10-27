// Try to implement some functional patterns
// Map, fold, filter

#include <stdlib.h>
#include <stdio.h>

int square(int n) {
    return n * n;
}

int add(int a, int b) {
    return a + b;
}

int even(int n) {
    return (n % 2) == 0;
}

void prINT(int n) {
    printf("%i ", n);
}

struct ArrayWithLength {
    int length;
    int* value;
} typedef ArrayWithLength;

void print_ArrayWithLength(ArrayWithLength arr) {
    printf("%i", arr.value[0]);
    for (int i = 1; i < arr.length; i++) {
        printf(" %i", arr.value[i]);
    }
    printf("\n");
}

// Map a function to an array
ArrayWithLength array_map(ArrayWithLength array, int (* mapping)(int)) {
    ArrayWithLength* newArray = malloc(sizeof(ArrayWithLength));
    newArray->length = array.length;
    int* value = malloc(sizeof(int) * array.length);
    for (int i = 0; i < newArray->length; i++) {
        value[i] = mapping(array.value[i]);
    }
    newArray->value = value;
    return *newArray;
}

void array_MapAction(ArrayWithLength array, void (* action)(int)) {
    for (int i = 0; i < array.length; i++) {
        action(array.value[i]);
    }
}

// Perform a fold on an array
int array_fold(ArrayWithLength array, int (* folder)(int, int), int initVal) {
    int acc = initVal;
//    printf("initVal: %i\n", acc);
    for (int i = 0; i < array.length; i++) {
        acc = folder(acc, array.value[i]);
//        printf("i: %i, arr[%i]: %i, acc: %i\n", i, i, array.value[i],acc);
    }
    return acc;
}

// Filter an array based on a predicate
ArrayWithLength array_filter(ArrayWithLength array, int (* predicate)(int)) {
    // Start by finding where the predicate is true
    ArrayWithLength predArray = array_map(array, predicate);

    // Find number of hits
    int len = array_fold(predArray, add, 0);

    // Construct array of return values
    int* retArr = malloc(sizeof(int) * len);
    for (int i = 0, next = 0; i < array.length; i++) {
        if (!predArray.value[i]) continue;
        retArr[next] = array.value[i];
        next++;
    }

    ArrayWithLength* returnArray = malloc(sizeof(ArrayWithLength));
    returnArray->length = len;
    returnArray->value = retArr;
    return *returnArray;
}

int main() {
#define LEN 10
    int initialValues[LEN];
    for (int i = 0; i < LEN; i++) {
        initialValues[i] = i;
    }
    ArrayWithLength InitialArray = {LEN, initialValues};
    array_MapAction(InitialArray, prINT);
    printf("\n");

#define Filter
#ifdef Filter
    ArrayWithLength filtered = array_filter(InitialArray, &even);
    print_ArrayWithLength(filtered);
#endif

#define MapFold
#ifdef MapFold
    ArrayWithLength Mapped = array_map(InitialArray, square);
    print_ArrayWithLength(Mapped);

    int sum = array_fold(Mapped, add, 0);
    printf("%i\n", sum);
#endif
    return 0;
}
