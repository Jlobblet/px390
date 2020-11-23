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
// Remove unnecessary free (the operating system can handle that since it is at the end of main)
// Move size of U and V arrays to variable
// Use stdbool.h for booleans
// Simplify loop counter for line 88
// Initialise V as an array, not an integer
// Move dUdx inside the loop to simplify
// Properly bracket grid spacing
// Initialise U with cos instead of sin
// Use upwinding method to solve
// Add output of initial conditions (t = 0)
// Allocate correct amount of memory for each array
// Calculate time_spacing variable to ensure stability
// Remove erroneous second increase of current_time
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

# define M_PI 3.14159265358979323846 // pi

// The following line does not contain a bug.
void read_input(double* C, double* gamma, double* L, int* nx, double* t_F, double* t_out);

int main() {
    // Parameters

    // Equation coefficients
    // All inputs can be assumed to be positive
    double C;
    double gamma;
    double domain_length;
    int number_points;
    double simulation_time;
    double output_timestep;

    // Read in from file
    read_input(&C, &gamma, &domain_length, &number_points, &simulation_time, &output_timestep);

    // Grid spacing
    double grid_spacing;
    if (number_points > 1) {
        grid_spacing = domain_length / (number_points - 1);
    } else {
        printf("Cannot solve with a single point along space.\n");
        exit(1);
    }
    // Small time step for stability.
    // Stable if C * time_spacing / grid_spacing < 1, i.e. stability_factor should be less than 1.
    const double stability_factor = 0.1;
    double time_spacing = grid_spacing * stability_factor / C;

    // Grid Storage
    // U at current and next timestep
    double* U, * U_next;
    // V at current and next timestep
    double* V, * V_next;

    // Allocate memory according to size of number_points
    size_t arr_size = number_points * sizeof(double);
    U = malloc(arr_size);
    V = malloc(arr_size);
    U_next = malloc(arr_size);
    V_next = malloc(arr_size);

    // initialisation
    // Initialise U
    for (int j = 0; j < number_points; j++) {
        double x = j * grid_spacing;
        U[j] = exp(cos(2 * M_PI * x / domain_length));
    }
    // Initialise V
    for (int j = 0; j < number_points; j++) {
        V[j] = 0;
    }


    double next_output_time = 0.0;
    double current_time = 0.0;

    // Loop over timesteps
    while (current_time < simulation_time) {
        double dt0 = time_spacing;
        bool output = false;
        // If we would overrun the next output step, reduce the timestep.
        if (current_time + dt0 > next_output_time) {
            dt0 = next_output_time - current_time;
            output = true;
        }

        // Loop over points
        for (int j = 0; j < number_points; j++) {
            // Centred space index for a periodic boundary.
            int jm = (j + number_points - 1) % number_points;
            // Finite difference evaluation of gradient.
            double slope = (U[j] - U[jm]) / grid_spacing;
            // Update pointwise
            // Since C and gamma can be assumed to be positive, no need to account for it.
            U_next[j] = U[j] - slope * dt0 * C;
            V_next[j] = V[j] + gamma * dt0 * (U[j] - V[j]);
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
