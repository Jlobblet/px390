#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef nenneke
#include <mkl_lapacke.h>
#else

#include <lapacke.h>

#endif

// Input

// Using a struct to store the input parameters since it makes function signatures nicer and keeps the
// variables together in a logical grouping.

typedef struct {
    double domain_length; // aka L
    int number_points; // N
    double advection_velocity; // nu
    double decay_rate; // tau
    double c; // value of P and Q at the right boundary x = L
} input_parameters;

// The read input functions return a bool
// true if they succeed, false if they fail.
// This is so that the piece of code that calls these functions can choose how to handle failure,
// instead of the functions forcing an exit.
// Ideally I would use a Result monad here but that's a bit too much work in C.

bool read_input(input_parameters* s) {
    FILE* input_file;
    // Using a const char over #define here since it has a symbol in the debug table
    const char INPUT_FILE[] = "input.txt";
    // This variable keeps track of whether the function should continue evaluating at each point.
    // This prevents needing to write
    //  fclose(input_file);
    //  return false;
    // many times, and instead it can be written once at the end.
    bool r = true;
    if (!(input_file = fopen(INPUT_FILE, "r"))) {
        printf("Error opening input file %s.\n", INPUT_FILE);
        r = false;
    }
    // By using the && operator, the right hand expression is not evaluated if the left hand expression is
    // 0 (or false, since false is just defined to be 0).
    if (r && fscanf(input_file,
                    "%lf %d %lf %lf %lf",
                    &s->domain_length,
                    &s->number_points,
                    &s->advection_velocity,
                    &s->decay_rate,
                    &s->c)
             != 5) {
        printf("Error reading input.\n");
        r = false;
    }
    // Perform input validation here, so that at any later step of the program it is guaranteed that this function's
    // return is valid
    if (r && *&s->number_points < 3) {
        printf("Number of grid points must be >= 3 (was %i).\n", *&s->number_points);
        r = false;
    }
    fclose(input_file);
    return r;
}

bool read_coefficients(double* D, double* S) {
    FILE* coefficients_file;
    const char COEFFICIENTS_FILE[] = "coefficients.txt";
    bool r = true;
    if (!(coefficients_file = fopen(COEFFICIENTS_FILE, "r"))) {
        printf("Error opening coefficients file %s.\n", COEFFICIENTS_FILE);
        r = false;
    }
    // Read each line until EOF, putting numbers into the current element arrays D and S
    while (r && EOF != fscanf(coefficients_file, "%lf %lf\n", D, S)) {
        // Increment the pointers D and S so that they now point to the next element in the array to be written to
        D++;
        S++;
    }
    fclose(coefficients_file);
    return r;
}

// Band matrix

typedef struct {
    // Number of columns in band matrix
    long n_columns;
    // Number of rows in matrix (equal to number of bands in original matrix)
    long n_band_rows;
    // Number of bands above diagonal
    long n_bands_upper;
    // Number of bands below diagonal
    long n_bands_lower;
    // Storage for matrix in banded format
    double* array;
    // Number of rows of inverse matrix
    long n_band_rows_inv;
    // Storage for inverse matrix
    double* array_inv;
    // Additional inverse information
    int* ipiv;
} band_matrix;

bool init_band_matrix(band_matrix* bm, long n_bands_upper, long n_bands_lower, long n_columns) {

    bm->n_columns = n_columns;
    bm->n_band_rows = n_bands_upper + n_bands_lower + 1;
    bm->n_bands_upper = n_bands_upper;
    bm->n_bands_lower = n_bands_lower;
    bm->n_band_rows_inv = bm->n_bands_upper * 2 + bm->n_bands_lower + 1;

    size_t array_size = sizeof(double) * bm->n_band_rows * bm->n_columns;
    size_t array_inv_size = sizeof(double) * (bm->n_band_rows + bm->n_bands_lower) * bm->n_columns;
    size_t ipiv_size = sizeof(int) * bm->n_columns;

    bm->array = malloc(array_size);
    bm->array_inv = malloc(array_inv_size);
    bm->ipiv = malloc(ipiv_size);

    if (bm->array == NULL || bm->array_inv == NULL || bm->ipiv == NULL) {
        printf("Failed to allocate %lu bytes of memory for band matrix.\n", array_size + array_inv_size + ipiv_size);
        return false;
    }

    for (long i = 0; i < bm->n_band_rows * bm->n_columns; i++) {
        bm->array[i] = 0.0;
    }

    return true;
}

void finalise_band_matrix(band_matrix* bm) {
    free(bm->array);
    free(bm->array_inv);
    free(bm->ipiv);
}

// Main

int main() {
    input_parameters params;
    // Since we left error handling to the call site, we must exit the program here if the function failed.
    if (!read_input(&params)) { return 1; }

    size_t array_size = sizeof(double) * params.number_points;
    double* D = malloc(array_size);
    double* S = malloc(array_size);
    // Ensure that we were allocated the memory asked for.
    if (D == NULL || S == NULL) {
        // array_size * 2 since if either D or S failed to allocate the program will exit.
        // We don't know which failed, but it doesn't matter - we need both for the program to work.
        printf("Failed to allocated %lu bytes memory", array_size * 2);
        return 1;
    }
    if (!read_coefficients(D, S)) { return 1; }

    for (int i = 0; i < params.number_points; i++) {
        printf("D[%i] = %lf; S[%i] = %lf\n", i, D[i], i, S[i]);
    }
    free(D);
    free(S);
    return 0;
}
