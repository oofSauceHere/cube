#ifndef MATRIX_H
#define MATRIX_H

#include <ostream>

class Matrix
{
protected:
    int m_, n_, det_;
    bool detFound_;
    double **X_;
    // bool REF_c, RREF_c, inv_c ???
    // Matrix REF_X, RREF_X, inv_X ???
public:
    // constructors/destructors
    Matrix(int m, int n, double **arr);
    Matrix(const Matrix &mat); // copy constructor (necessary for deep copy)
    ~Matrix();

    // overloads
    const double *operator[](int i);
    Matrix &operator=(const Matrix &mat);
    Matrix operator+(const Matrix &B);
    Matrix operator*(const Matrix &B);
    bool operator==(const Matrix &B);
    friend Matrix operator*(int s, const Matrix &A);                    // in matrixFunctions.cpp
    friend std::ostream &operator<<(std::ostream &os, const Matrix &A); // in matrixFunctions.cpp

    // getters/display function
    int getM() const;
    int getN() const;

    // property-related functions
    double dotProduct(const Matrix &B, int i, int j) const; // should probably deal with vectors

    // elementary operations
    void swap(int r1, int r2);
    void rowMultiply(double s, int r);
    void rowAdd(double s, int r1, int r2);

    // other operations
    Matrix transpose();

    // linear algebra-related
    void REF();
    void RREF(); // should these permanently change the target matrix?
    // double determinant();
    Matrix inverse();
    // Matrix solve(Matrix b);
};

#endif