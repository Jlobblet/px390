// Write a function with prototype:
//
// void en_pot(double *posx, double *posy, double *posz, long ncharges, double *res)
//
// Given three arrays, posx, posy, posz, which represent the x, y and z positions of a set of charges,
// calculate the total potential energy (in normalised units) as given below, and place this value
// in the address pointed to by res
//
// (That is, on output *res should be equal to the potential energy).
// The input parameter ncharges gives the total number of charges. The charges will not be on top of each other.
//
// (There was an image here. Replaced with LaTeX.)
// \text{Given three arrays, posx, posy, posz, which represent the x, y, and z positions
// of a set of charges, calculate the total potential energy given by}\\
// P = \frac{1}{2}\sum_i\sum_{j\neq i}\frac{1}{|\boldsymbol{r}_i-\boldsymbol{r}_j|}\\
// \text{Where }\boldsymbol{r}_i = (\text{posx}_i, \text{posy}_i, \text{posz}_i)
// \text{, and the norm in the denominator gives the length of the vector.}

#include <stdio.h>

// begin question

#include <math.h>

double euclidean_3_norm(double x, double y, double z) {
    return sqrt(x * x + y * y + z * z);
}

void en_pot(double* posx, double* posy, double* posz, long ncharges, double* res) {
    double sum = 0;
    for (long i = 0; i < ncharges; i++) {
        for (long j = 0; j < ncharges; j++) {
            if (j == i) { continue; }
            sum += (1 / euclidean_3_norm(posx[i] - posx[j], posy[i] - posy[j], posz[i] - posz[j]));
        }
    }
    *res = 0.5 * sum;
}

// end question

int main() {
#define ncharges 2
    double posx[ncharges] = {1, -1};
    double posy[ncharges] = {0, 0};
    double posz[ncharges] = {0, 0};
    double res;
    en_pot(posx, posy, posz, ncharges, &res);
    printf("%4.2f\n", res);
    return 0;
}
