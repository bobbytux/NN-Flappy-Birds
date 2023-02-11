// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

class Matrix {

public:

    int rows;
    int cols;
    std::vector<std::vector<double>> content;

    Matrix(int rows_, int cols_);

    ~Matrix();

    void print();
    void randomize();

    Matrix transpose();

    void add(double);
    void add(Matrix);
    static Matrix add(Matrix, Matrix);

    void substract(double);
    void substract(Matrix);
    static Matrix substract(Matrix, double);
    static Matrix substract(Matrix m1, Matrix m2);

    void multiply(double);
    Matrix multiply(Matrix);
    static Matrix multiply(Matrix, Matrix);

    static Matrix adamar_multiply(Matrix, Matrix);

    void map(double (*)(double));
    static Matrix map(Matrix, double (*)(double));

    void operator = (Matrix);
};

#endif // MATRIX_H