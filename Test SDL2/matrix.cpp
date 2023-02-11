// matrix.cpp
// PC - 20/10/2021

#include <iostream>
#include <string>

#include "matrix.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

Matrix::Matrix(int rows_, int cols_) : cols(cols_), rows(rows_) {
    // Create a zero content row
    std::vector<double> row = {};
    for (int i = 0; i < cols_; i++) {
        row.push_back(0.0f);
    }
    // create the rows with lines's zero contents
    for (int i = 0; i < rows_; i++) {
        content.push_back(row);
    }
    row.clear();
}

Matrix::~Matrix() {
    for (int i = 0; i < this->rows; i++) {
        content[i].clear();
    }
    content.clear();
}

void Matrix::print() {

    fprintf(stdout, "\n\r");

    std::string line;
    // char buffer[64];
    // char line[256];

    for (int i = 0; i < rows; i++) {
        // line[0] = '\0';
        line.erase();
        for (int j = 0; j < cols; j++) {
            // sprintf(buffer, "%f ", content[i][j]);
            // strcat(line, buffer);
            line += content[i][j];
            line += " ";
        }
        // SDL_Log(line);
        // fprintf(stdout, "%s\n\r", line);
        std::cout << line << std::endl;
    }
}

void Matrix::randomize() {
    // srand( time(NULL) );
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->content[i][j] = ((double)(rand() % 20000) / 10000.0f) - 1.0f;
        }
    }
}

void Matrix::add(double n) {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->content[i][j] += n;
        }
    }
}

void Matrix::add(Matrix m2) {
    if ((m2.rows != this->rows) || (m2.cols != this->cols)) {
        fprintf(stderr, "Add() : Matrices must be of the same size !");
        return;
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->content[i][j] += m2.content[i][j];
        }
    }
}

Matrix Matrix::add(Matrix m1, Matrix m2) { // static

    if ((m2.rows != m1.rows) || (m2.cols != m1.cols)) {
        fprintf(stderr, "Add() : Matrices must be of the same size !");
        return m1;
    }

    Matrix result(m1.rows, m1.cols);

    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            result.content[i][j] = m1.content[i][j] + m2.content[i][j];
        }
    }

    return result;
}

void Matrix::substract(double n) {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->content[i][j] -= n;
        }
    }
}

Matrix Matrix::substract(Matrix m1, double n) { // static

    Matrix result(m1.rows, m1.cols);

    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            result.content[i][j] = m1.content[i][j] - n;
        }
    }

    return result;
}

void Matrix::substract(Matrix m2) {
    if ((m2.rows != this->rows) || (m2.cols != this->cols)) {
        fprintf(stderr, "substract() : Matrices must be of the same size !");
        return;
    }
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->content[i][j] -= m2.content[i][j];
        }
    }
}

Matrix Matrix::substract(Matrix m1, Matrix m2) { // static

    if ((m2.rows != m1.rows) || (m2.cols != m1.cols)) {
        fprintf(stderr, "substract() : Matrices must be of the same size !");
        return m1;
    }

    Matrix result(m1.rows, m1.cols);

    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            result.content[i][j] = m1.content[i][j] - m2.content[i][j];
        }
    }

    return result;
}

void Matrix::map(double (*function)(double)) {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->content[i][j] = function(this->content[i][j]);
        }
    }
}

Matrix Matrix::map(Matrix m1, double (*function)(double)) { // static
    Matrix result(m1.rows, m1.cols);
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            result.content[i][j] = function(m1.content[i][j]);
        }
    }
    return result;
}

void Matrix::multiply(double n) {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->content[i][j] *= n;
        }
    }
}

Matrix Matrix::multiply(Matrix m) {
    return(Matrix::multiply(*this, m));
}

Matrix Matrix::multiply(Matrix m1, Matrix m2) { // static

    Matrix m_result(m1.rows, m2.cols);

    if (m1.cols != m2.rows) {
        fprintf(stderr, "Matrix::multiply() : Nb cols of first matrix must be the same as nb rows of the second !");
        return m_result;
    }

    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m2.cols; j++) {
            double sum = 0.0f;
            for (int k = 0; k < m2.rows; k++) {
                sum += m1.content[i][k] * m2.content[k][j];
            }
            m_result.content[i][j] = sum;
        }
    }

    return m_result;
}

Matrix Matrix::adamar_multiply(Matrix m1, Matrix m2) { // static

    Matrix m_result(m1.rows, m1.cols);

    if ((m1.cols != m2.cols) || (m1.rows != m2.rows)) {
        fprintf(stderr, "Matrix::adamar_multiply() : the two matrices must have the same dimentions !");
        return m_result;
    }

    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m2.cols; j++) {
            m_result.content[i][j] = m1.content[i][j] * m2.content[i][j];
        }
    }

    return m_result;
}

Matrix Matrix::transpose() {

    // Create a new matrix with transposed content
    Matrix m2(this->cols, this->rows);
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            m2.content[j][i] = this->content[i][j];
        }
    }

    return m2;
}

void Matrix::operator = (Matrix m2) {
    if ((m2.rows != this->rows) || (m2.cols != this->cols)) {
        fprintf(stderr, "Operator = : Matrices must be of the same size !");
        return;
    }

    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            this->content[i][j] = m2.content[i][j];
        }
    }
}


