#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef nenneke
#include <mkl_lapacke.h>
#else

#include <lapacke.h>

#endif

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
// instead of the functions forcing a crash.
// Ideally I would use a Result monad here but that's a bit too much work in C.

bool read_input(input_parameters* s) {
    FILE* input_file;
    // Using a const char over #define here since it has a symbol in the debug table
    const char INPUT_FILE[] = "input.txt";
    if (!(input_file = fopen(INPUT_FILE, "r"))) {
        printf("Error opening input file %s.\n", INPUT_FILE);
        fclose(input_file);
        return false;
    }
    if (fscanf(input_file,
               "%lf %d %lf %lf %lf",
               &s->domain_length,
               &s->number_points,
               &s->advection_velocity,
               &s->decay_rate,
               &s->c)
        != 5) {
        printf("Error reading input.\n");
        fclose(input_file);
        return false;
    }
    // Perform input validation here, so that at any later step of the program it is guaranteed that this function's
    // return is valid
    if (*&s->number_points < 3) {
        printf("Number of grid points must be >= 3 (was %i).\n", *&s->number_points);
        fclose(input_file);
        return false;
    }
    fclose(input_file);
    return true;
}

bool read_coefficients(double* D, double* S) {
    FILE* coefficients_file;
    const char COEFFICIENTS_FILE[] = "coefficients.txt";
    if (!(coefficients_file = fopen(COEFFICIENTS_FILE, "r"))) {
        printf("Error opening coefficients file %s.\n", COEFFICIENTS_FILE);
        fclose(coefficients_file);
        return false;
    }
    // Read each line until EOF, putting numbers into the current element arrays D and S
    while (EOF != fscanf(coefficients_file, "%lf %lf\n", D, S)) {
        // Increment the pointers D and S so that they now point to the next element in the array to be written to
        D++;
        S++;
    }
    fclose(coefficients_file);
    return true;
}

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
