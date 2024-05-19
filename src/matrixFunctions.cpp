#include "matrixFunctions.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

// user input
Matrix readMatrix()
{
    int m, n;
    cout << "Input m and n: " << endl;
    cin >> m >> n;
    cout << "\n";

    double **arr = new double *[m];
    cout << "Input matrix: " << endl;
    for (int i = 0; i < m; i++)
    {
        arr[i] = new double[n];
        for (int j = 0; j < n; j++)
        {
            cin >> arr[i][j];
        }
    }
    cout << "\n";

    Matrix mat(m, n, arr);
    return mat;
}

// overloads
Matrix operator*(int s, const Matrix &A)
{
    double **arr = new double *[A.m_];
    for (int i = 0; i < A.m_; i++)
    {
        arr[i] = new double[A.n_];
        for (int j = 0; j < A.n_; j++)
        {
            arr[i][j] = s * A.X_[i][j];
        }
    }
    return Matrix(A.m_, A.n_, arr);
}

std::ostream &operator<<(std::ostream &os, const Matrix &A)
{
    int *maxw = new int[A.n_];
    for (int i = 0; i < A.n_; i++)
    {
        maxw[i] = 0;
        for (int j = 0; j < A.m_; j++)
        {
            int digits = floor(log10(abs(A.X_[j][i]))) + 1;
            if (digits < 1)
                digits = 1;
            if (A.X_[j][i] < 0)
                digits++;
            if (digits > maxw[i])
            {
                maxw[i] = digits;
            }
        }
    }

    for (int i = 0; i < A.m_; i++)
    {
        os << "|";
        for (int j = 0; j < A.n_; j++)
        {
            if (A.X_[i][j] == 0.0)
            {
                os << setprecision(1) << fixed << setw(maxw[j] + 3) << abs(A.X_[i][j]);
            }
            else
            {
                os << setprecision(1) << fixed << setw(maxw[j] + 3) << A.X_[i][j];
            }
        }
        os << setw(3) << "|\n";
    }

    delete[] maxw;
    return os;
}

// constant matrix generation
Matrix zero(int m, int n)
{
    double **arr = new double *[m];
    for (int i = 0; i < m; i++)
    {
        arr[i] = new double[n];
        for (int j = 0; j < n; j++)
        {
            arr[i][j] = 0;
        }
    }
    return Matrix(m, n, arr);
}

Matrix identity(int n)
{
    double **arr = new double *[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = new double[n];
        for (int j = 0; j < n; j++)
        {
            arr[i][j] = 0;
            if (i == j)
                arr[i][j] = 1;
        }
    }
    return Matrix(n, n, arr);
}

// convenience
void cleanup(int m, double **arr)
{
    for (int i = 0; i < m; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;
}