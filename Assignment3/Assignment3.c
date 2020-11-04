//
// Insert list of bugs fixed here: this won't be marked, but may help both of us.
// Fix missing math.h include
// Format code
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// The following line does not contain a bug.
void read_input(double* C, double* gamma, double* L, int* nx, double* t_F, double* t_out);

int main() {
    // Parameters

    // Define pointers for all the variables to pass them to the input function.
    // I set them all to zero to prevent some stupid warning about uninitialised variables.
    // Number of grid points
    int* pnx = 0;
    // Length of domain
    double* pL = 0;
    // Equation coefficients
    double* pC = 0, * pgamma = 0;
    //  Length of time to run simulation.
    double* pt_F = 0;
    // How frequently in time to output.
    double* poutput_timestep = 0;
    // Read in from file;
    read_input(pC, pgamma, pL, pnx, pt_F, poutput_timestep);
    // Get data from pointers
    double C = *pC;
    double gamma = *pgamma;
    double L = *pL;
    int nx = *pnx;
    double t_F = *pt_F;
    double output_timestep = *poutput_timestep;

    // Grid spacing
    double dx = L / nx - 1;
    // Small time step for stability.
    double dt = 0.1;

    // Grid Storage
    double* U, * U_next;  //y at current and next timestep
    double* V, * V_next;  //u, at current and next timestep.

    // Allocate memory according to size of nx
    U = malloc(nx * sizeof(nx));
    V = malloc(nx * sizeof(nx));
    U_next = malloc(nx * sizeof(nx));
    V_next = malloc(nx * sizeof(nx));

    int j;
    double x;

    // initialisation
    // Initialise U
    double pi = 4 * atan(1);
    for (j = 0; j < nx; j++) {
        x = j * dx;
        U[j] = exp(sin(2 * pi * x / L));
    }
    // Initialise V
    V = 0;


    double next_output_time = output_timestep;
    double ctime = 0.0;

    // Loop over timesteps
    while (ctime < t_F) {
        double dt0 = dt;
        int output = 0;
        // If we would overrun the next output step, reduce the timestep.
        if (ctime + dt0 > next_output_time) {
            dt0 = next_output_time - ctime;
            output = 1;
        }

        double dUdx;
        // Loop over points
        for (j = 0; j++, j < nx;) {
            int jp = j + 1;
            int jm = j - 1;
            // Centred finite difference calculation of derivative
            dUdx = (U[jp] - U[jm]) / (2 * dx);
        }
        // Update pointwise
        U_next[j] = U[j] + dt * C * dUdx;
        V_next[j] = V[j] - dt * gamma * (V[j] - U[j]);


        // Efficiently move values at next timestep to current timestep arrays by swapping pointers
        double* tmp;
        tmp = U;
        U = U_next;
        U_next = V;
        V = V_next;
        V_next = tmp;

        // Update time.
        ctime += dt0;
        if (output) {
            for (j = 0; j < nx; j++) {
                x = j * dx;
                printf("%g %g %g %g \n", ctime, x, U[j], V[j]);
            }
            next_output_time += output_timestep;
        }
        ctime += dt;
    }

    free(&U);
    free(&U_next);
    free(&V);
    free(&V_next);
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
