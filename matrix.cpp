#include "matrixFunctions.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

// exception handling
struct DimensionMismatchException : public exception
{
    const char *what()
    {
        return "Dimensions mismatched! Cannot perform operation.";
    }
};

struct NonInvertibleException : public exception
{
    const char *what()
    {
        return "Matrix not invertible! Cannot perform operation.";
    }
};

// constructors/destructor
Matrix::Matrix(int m, int n, double **arr) : m_(m), n_(n), det_(0), detFound_(false)
{
    X_ = new double *[m_];
    for (int i = 0; i < m_; i++)
    {
        X_[i] = new double[n_];
        for (int j = 0; j < n_; j++)
        {
            X_[i][j] = arr[i][j];
        }
    }

    cleanup(m, arr);
}

Matrix::Matrix(const Matrix &mat) : m_(mat.getM()), n_(mat.getN()), det_(0), detFound_(false)
{
    X_ = new double *[m_];
    for (int i = 0; i < m_; i++)
    {
        X_[i] = new double[n_];
        for (int j = 0; j < n_; j++)
        {
            X_[i][j] = mat.X_[i][j];
        }
    }
}

Matrix::~Matrix()
{
    for (int i = 0; i < m_; i++)
    {
        delete[] X_[i];
    }
    delete[] X_;
}

// overloads
const double *Matrix::operator[](int i)
{
    return X_[i];
}

Matrix &Matrix::operator=(const Matrix &mat)
{
    if (this != &mat)
    {
        int m = mat.getM();
        int n = mat.getN();

        if (X_ != nullptr)
        {
            for (int i = 0; i < m_; i++)
            {
                delete[] X_[i];
            }
            delete[] X_;
        }

        X_ = new double *[m];
        for (int i = 0; i < m; i++)
        {
            X_[i] = new double[n];
            for (int j = 0; j < n; j++)
            {
                X_[i][j] = mat.X_[i][j];
            }
        }
    }

    return *this;
}

Matrix Matrix::operator+(const Matrix &B)
{
    if (m_ != B.m_ || n_ != B.n_)
        throw DimensionMismatchException();

    double **arr = new double *[m_];
    for (int i = 0; i < m_; i++)
    {
        arr[i] = new double[n_];
        for (int j = 0; j < n_; j++)
        {
            arr[i][j] = X_[i][j] + B.X_[i][j];
        }
    }

    return Matrix(m_, n_, arr);
}

Matrix Matrix::operator*(const Matrix &B)
{
    if (n_ != B.m_)
        throw DimensionMismatchException();

    double **arr = new double *[m_];
    for (int i = 0; i < m_; i++)
    {
        arr[i] = new double[B.n_];
        for (int j = 0; j < B.n_; j++)
        {
            arr[i][j] = this->dotProduct(B, i, j);
        }
    }

    return Matrix(m_, B.n_, arr);
}

bool Matrix::operator==(const Matrix &B)
{
    for (int i = 0; i < n_; i++)
    {
        for (int j = 0; j < m_; j++)
        {
            if (X_[i][j] != B.X_[i][j])
                return false;
        }
    }

    return true;
}

// getters/display functions
int Matrix::getM() const
{
    return m_;
}

int Matrix::getN() const
{
    return n_;
}

// property-related functions
double Matrix::dotProduct(const Matrix &B, int i, int j) const
{
    double sum = 0;
    for (int dim = 0; dim < n_; dim++)
    {
        sum += X_[i][dim] * B.X_[dim][j];
    }
    return sum;
}

// elementary operations
void Matrix::swap(int r1, int r2)
{
    for (int j = 0; j < n_; j++)
    {
        double temp = X_[r1][j];
        X_[r1][j] = X_[r2][j];
        X_[r2][j] = temp;
    }
}

void Matrix::rowMultiply(double s, int r)
{
    for (int j = 0; j < n_; j++)
    {
        X_[r][j] *= s;
    }
}

void Matrix::rowAdd(double s, int r1, int r2)
{
    for (int j = 0; j < n_; j++)
    {
        X_[r1][j] += s * X_[r2][j];
    }
}

// other operations
Matrix Matrix::transpose()
{
    double **arr = new double *[n_];
    for (int i = 0; i < n_; i++)
    {
        arr[i] = new double[m_];
        for (int j = 0; j < m_; j++)
        {
            arr[i][j] = X_[j][i];
        }
    }

    return Matrix(n_, m_, arr);
}

void Matrix::REF()
{
    int r = 0;
    for (int j = 0; j < n_; j++)
    {
        cout << *this << "\n";

        if (X_[r][j] == 0)
        {
            int new_r = r;
            while (new_r < m_ && X_[new_r][j] == 0)
            {
                new_r++;
            }
            if (new_r == m_)
                continue;
            else
            {
                swap(r, new_r);
                cout << *this << "\n";
            }
        }

        if (X_[r][j] == 0)
            continue;

        for (int i = r + 1; i < m_; i++)
        {
            rowAdd(-1 * X_[i][j] / X_[r][j], i, r);
        }
        r++;
    }
}

void Matrix::RREF()
{
    int r = 0;
    for (int j = 0; j < n_; j++)
    {
        cout << *this << "\n";

        if (X_[r][j] == 0)
        {
            int new_r = r;
            while (new_r < m_ && X_[new_r][j] == 0)
            {
                new_r++;
            }
            if (new_r == m_)
                continue;
            else
            {
                swap(r, new_r);
                cout << *this << "\n";
            }
        }

        rowMultiply(1.0 / X_[r][j], r);
        for (int i = 0; i < m_; i++)
        {
            if (i != r)
            {
                rowAdd(-1 * X_[i][j], i, r);
            }
        }
        r++;
    }
}

Matrix Matrix::inverse()
{
    if (n_ != m_)
        throw NonInvertibleException();

    Matrix tmp(*this);
    Matrix inv = identity(n_);

    int r = 0;
    for (int j = 0; j < n_; j++)
    {
        if (tmp[r][j] == 0)
        {
            int new_r = r;
            while (new_r < m_ && tmp[new_r][j] == 0)
            {
                new_r++;
            }
            if (new_r == m_)
                continue;
            else
            {
                inv.swap(r, new_r);
                tmp.swap(r, new_r);
            }
        }

        inv.rowMultiply(1.0 / tmp[r][j], r);
        tmp.rowMultiply(1.0 / tmp[r][j], r);
        for (int i = 0; i < m_; i++)
        {
            if (i != r)
            {
                inv.rowAdd(-1 * tmp[i][j], i, r);
                tmp.rowAdd(-1 * tmp[i][j], i, r);

                // debugging
                // cout << i << endl;
                // cout << tmp << endl;
                // cout << inv << endl;
            }
        }
        r++;
    }

    if (r < m_)
    {
        det_ = 0;
        detFound_ = true;
        throw NonInvertibleException();
    }

    return inv;
}