#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <mkl_lapacke.h>

// Input

// Using a struct to store the input parameters since it makes function signatures nicer and keeps the
// variables together in a logical grouping.

typedef struct {
    double domain_length; // aka L
    long number_points; // N
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
                    "%lf %ld %lf %lf %lf",
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
        printf("Number of grid points must be >= 3 (was %ld).\n", *&s->number_points);
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

bool write_output(int number_points, double grid_spacing, double* P, double* Q) {
    FILE* output_file;
    const char OUTPUT_FILE[] = "output.txt";
    bool r = true;
    if (!(output_file = fopen(OUTPUT_FILE, "w"))) {
        printf("Error creating or editing output file %s.\n", OUTPUT_FILE);
        r = false;
    }
    if (r) {
        for (int i = 0; i < number_points; i++) {
            fprintf(output_file, "%lf %lf %lf\n", grid_spacing * i, P[i], Q[i]);
        }
    }
    fclose(output_file);
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
    // Adding 1 to account for the diagonal
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

    for (long i = 0L; i < bm->n_band_rows * bm->n_columns; i++) {
        bm->array[i] = 0.0;
    }

    for (long i = 0L; i < (bm->n_band_rows + bm->n_bands_lower) * bm->n_columns; i++) {
        bm->array_inv[i] = 0.0;
    }

    return true;
}

void finalise_band_matrix(band_matrix* bm) {
    free(bm->array);
    free(bm->array_inv);
    free(bm->ipiv);
}

// This function returns NULL instead of crashing so that the call site can determine what to do in case of error.
// Again a Result or Option/Maybe monad would be ideal here.

double* getp(band_matrix* bm, long row, long column) {
    if (row < 0 || column < 0 || row >= bm->n_columns || column >= bm->n_columns) {
        printf("Index out of bounds: given (%ld, %ld), size is (%ld, %ld)\n", row, column, bm->n_columns,
               bm->n_columns);
        return NULL;
    }

    long band_number = bm->n_bands_upper + row - column;
    return &bm->array[bm->n_band_rows * column + band_number];
}

bool setv(band_matrix* bm, long row, long column, double value) {
    double* ptr = getp(bm, row, column);
    if (ptr == NULL) {
        return false;
    }
    *ptr = value;
    return true;
}

void print_mat(band_matrix* bm) {
    for (long i = 0L; i < bm->n_columns; i++) {
        for (long j = 0L; j < bm->n_band_rows; j++) {
            printf("%lf      ", *getp(bm, j, i));
        }
        printf("\n");
    }
}


void print_mat2(band_matrix* bm) {
    for (long i = 0L; i < bm->n_columns; i++) {
        for (long j = 0L; j < bm->n_band_rows; j++) {
            printf("%ld %ld %g \n", i, j, bm->array[bm->n_band_rows * i + j]);
        }
    }
}

int solve_Ax_eq_b(band_matrix* bm, double* x, double* b) {
    for (long i = 0L; i < bm->n_columns; i++) {
        for (long band = 0L; band < bm->n_band_rows; band++) {
            bm->array_inv[bm->n_band_rows_inv * i + band + bm->n_bands_lower] = bm->array[bm->n_band_rows * i + band];
        }
        x[i] = b[i];
    }

    long n_rhs = 1L;
    long ldab = bm->n_bands_lower * 2L + bm->n_bands_upper + 1L;
    int info = LAPACKE_dgbsv(
            LAPACK_COL_MAJOR,
            bm->n_columns,
            bm->n_bands_lower,
            bm->n_bands_upper,
            n_rhs,
            bm->array_inv,
            ldab,
            bm->ipiv,
            b,
            bm->n_columns
    );

    return info;
}

// Main

int main() {
    input_parameters params;
    // Since we left error handling to the call site, we must exit the program here if the function failed.
    if (!read_input(&params)) { return 1; }

    // The matrix has to be square, so we must have a grid spacing equal to domain length divided by number of points - 1
    double grid_spacing = params.domain_length / (params.number_points - 1);
    double grid_spacing_sq = grid_spacing * grid_spacing;

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
    // Exit the program if reading coefficients fails
    if (!read_coefficients(D, S)) { return 1; }

    // Account for final boundary condition
    S[params.number_points - 1] = params.c;

    // Create band matrices
    // Exit program if either of these fail to be instantiated

    // Band matrix has 1 above diagonal,
    long n_bands_upper = 1L;
    // and 1 below diagonal.
    long n_bands_lower = 1L;
    // And the number of columns is the same as the number of points.
    // This is also the same as the number of rows in the original matrix.
    long n_cols = params.number_points;

    band_matrix P_matrix;
    if (!init_band_matrix(&P_matrix, n_bands_upper, n_bands_lower, n_cols)) { return 1; }

    band_matrix Q_matrix;
    if (!init_band_matrix(&Q_matrix, n_bands_upper, n_bands_lower, n_cols)) { return 1; }

    // Initial values for the P matrix

    // Initial boundary conditions
    setv(&P_matrix, 0L, 0L, params.decay_rate); // aka tau
    setv(&Q_matrix, 0L, 0L, 1.0);

    // Middle bit of the matrix
    for (long i = 1L; i < params.number_points - 1L; i++) {
        // Matrix for P
        // -ν/Δx - D(x_{i-1}) / Δx^2
        setv(&P_matrix, i, i - 1L,
             -params.advection_velocity / grid_spacing
             - D[i - 1] / grid_spacing_sq
        );
        // τ + ν/Δx - (D(x_i) + D(x_{i-1})) / Δx^2
        setv(&P_matrix, i, i,
             params.decay_rate
             + params.advection_velocity / grid_spacing
             + (D[i] + D[i - 1]) / grid_spacing_sq
        );
        // -D(x_i) / Δx^2
        setv(&P_matrix, i, i + 1L,
             -D[i] / grid_spacing_sq
        );
        // Matrix for Q
        // -ν/Δx - D(x_{i-1}) / Δx^2
        setv(&Q_matrix, i, i - 1L,
             -params.advection_velocity / grid_spacing
             - D[i - 1] / grid_spacing_sq
        );
        // ν/Δx + (D(x_i) + D(x_{i-1})) / Δx^2
        setv(&Q_matrix, i, i,
             params.advection_velocity / grid_spacing
             + (D[i] + D[i - 1]) / grid_spacing_sq
        );
        // -D(x_i) / Δx^2
        setv(&Q_matrix, i, i + 1L,
             -D[i] / grid_spacing_sq
        );
    }

    // Final boundary conditions
    setv(&P_matrix, P_matrix.n_columns - 1, P_matrix.n_columns - 1, 1.0);
    setv(&Q_matrix, Q_matrix.n_columns - 1, Q_matrix.n_columns - 1, 1.0);

    // Solve equation
    double* P = malloc(array_size);
    double* Q = malloc(array_size);
    double* P_decay = malloc(array_size);

    if (P == NULL || Q == NULL || P_decay == NULL) {
        printf("Failed to allocate %lu bytes of memory.\n", 3 * array_size);
        return 1;
    }

    solve_Ax_eq_b(&P_matrix, P, S);
    for (long i = 0L; i < P_matrix.n_columns; i++) {
        P_decay[i] = params.decay_rate * P[i];
    }
    // Account for the final boundary condition
    P_decay[P_matrix.n_columns - 1] = params.c;

    solve_Ax_eq_b(&Q_matrix, Q, P_decay);

    if (!write_output(params.number_points, grid_spacing, P, Q)) { return 1; }

    free(D);
    free(S);
    free(P);
    free(Q);
    free(P_decay);
    finalise_band_matrix(&P_matrix);
    finalise_band_matrix(&Q_matrix);
    return 0;
}
