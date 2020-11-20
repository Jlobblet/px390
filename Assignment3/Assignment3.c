//
// Insert list of bugs fixed here: this won't be marked, but may help both of us.
// Fix missing math.h include
// Format code
// Use reference to input variables instead of declaring useless pointers
// Refactor variable names
// Add pi macro
// Inline declarations of loop counters
// Merge declarations and assignments to variable x
// Fix incorrect variable names in comments
// Remove unnecessary free
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

# define M_PI 3.14159265358979323846 // pi

// The following line does not contain a bug.
void read_input(double* C, double* gamma, double* L, int* nx, double* t_F, double* t_out);

int main() {
    // Parameters

    // Equation coefficients
    double C;
    double gamma;
    // Length of domain
    double domain_length;
    // Number of points
    int number_points;
    // Length of time to run simulation
    double simulation_time;
    // How frequently in time to output
    double output_timestep;

    // Read in from file
    read_input(&C, &gamma, &domain_length, &number_points, &simulation_time, &output_timestep);

    // Grid spacing
    double grid_spacing = domain_length / number_points - 1;
    // Small time step for stability.
    double time_spacing = 0.1;

    // Grid Storage
    // U at current and next timestep
    double* U, * U_next;
    // V at current and next timestep
    double* V, * V_next;

    // Allocate memory according to size of number_points
    U = malloc(number_points * sizeof(number_points));
    V = malloc(number_points * sizeof(number_points));
    U_next = malloc(number_points * sizeof(number_points));
    V_next = malloc(number_points * sizeof(number_points));

    // initialisation
    // Initialise U
    for (int j = 0; j < number_points; j++) {
        double x = j * grid_spacing;
        U[j] = exp(sin(2 * M_PI * x / domain_length));
    }
    // Initialise V
    V = 0;


    double next_output_time = output_timestep;
    double current_time = 0.0;

    // Loop over timesteps
    while (current_time < simulation_time) {
        double dt0 = time_spacing;
        int output = 0;
        // If we would overrun the next output step, reduce the timestep.
        if (current_time + dt0 > next_output_time) {
            dt0 = next_output_time - current_time;
            output = 1;
        }

        double dUdx;
        // Loop over points
        for (int j = 0; j++, j < number_points;) {
            int jp = j + 1;
            int jm = j - 1;
            // Centred finite difference calculation of derivative
            dUdx = (U[jp] - U[jm]) / (2 * grid_spacing);
            // Update pointwise
            U_next[j] = U[j] + time_spacing * C * dUdx;
            V_next[j] = V[j] - time_spacing * gamma * (V[j] - U[j]);
        }


        // Efficiently move values at next timestep to current timestep arrays by swapping pointers
        double* tmp;
        tmp = U;
        U = U_next;
        U_next = V;
        V = V_next;
        V_next = tmp;

        // Update time.
        current_time += dt0;
        if (output) {
            for (int j = 0; j < number_points; j++) {
                double x = j * grid_spacing;
                printf("%g %g %g %g \n", current_time, x, U[j], V[j]);
            }
            next_output_time += output_timestep;
        }
        current_time += time_spacing;
    }
}

// The lines below don't contain any bugs! Don't modify them.
void read_input(double* C, double* gamma, double* L, int* nx, double* t_F, double* t_out) {
    FILE* infile;
    if (!(infile = fopen("input.txt", "r"))) {
        printf("Error opening file\n");
        exit(1);
    }
    if (6 != fscanf(infile, "%lf %lf %lf %d %lf %lf", C, gamma, L, nx, t_F, t_out)) {
        printf("Error reading parameters from file\n");
        exit(1);
    }
    fclose(infile);
}
