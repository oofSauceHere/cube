#ifndef MATRIXFUNCTIONS_H
#define MATRIXFUNCTIONS_H

#include "matrix.h"

// user input
Matrix readMatrix();

// constant matrix generation
Matrix zero(int m, int n);
Matrix identity(int n);

// convenience
void cleanup(int m, double **arr);

#endif