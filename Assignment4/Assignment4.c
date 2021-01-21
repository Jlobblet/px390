#include <stdio.h>
#include <stdlib.h>

#ifdef nenneke
#include <mkl_lapacke.h>
#else
#include <lapacke.h>
#endif

struct input_parameters {
    double domain_length; // aka L
    int number_points; // N
    double advection_velocity; // nu
    double decay_rate; // tau
    double c;
} typedef input_parameters;

void read_input(input_parameters *s) {
    FILE *input_file;
    if (!(input_file = fopen("input.txt", "r"))) {
        printf("Error opening input file.\n");
        exit(1);
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
        exit(1);
    }
    fclose(input_file);
}

void read_coefficients(double* D, double* S) {
    FILE *coefficients_file;
    if (!(coefficients_file = fopen("coefficients.txt", "r"))) {
        printf("Error opening coefficients file.\n");
        exit(1);
    }
    while (EOF != fscanf(coefficients_file, "%lf %lf\n", D, S)) {
        D++;
        S++;
    }
    fclose(coefficients_file);
}

int main() {
    input_parameters params;
    read_input(&params);
    size_t array_size = sizeof(double) * params.number_points;
    double* D = malloc(array_size);
    double* S = malloc(array_size);
    if (D == NULL || S == NULL) {
        printf("Failed to allocated %lu bytes memory", array_size * 2);
        exit(1);
    }
    read_coefficients(D, S);
    for (int i = 0; i < params.number_points; i++) {
        printf("D[%i] = %lf; S[%i] = %lf\n", i, D[i], i, S[i]);
    }
    free(D);
    free(S);
    return 0;
}
