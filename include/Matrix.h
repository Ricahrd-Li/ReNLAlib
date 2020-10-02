//
// Created by 李喆昊 on 2020/9/29.
//
#ifndef CODES_MATRIX_H
#define CODES_MATRIX_H
#include <iostream>
#include <vector>
#include <cassert>
#include "Vec.h"
using namespace std;

namespace NLA {
    class Matrix {
    public:
        Matrix(vector<vector<double>> v, bool isColPrior=false);
        Matrix(vector<Vec>);
        Matrix(vector<double>);
        Matrix(int nRow, int nCol, double num);
        Matrix(int nRow, int nCol); // Create a zero matrix
        Matrix(int nRow);

        vector<int> shape();

        Matrix transpose();

        Matrix setNum(double num);
        Matrix setIdentity();
        Matrix setZero();
        Matrix setTripleDiag(double lambda, double lambdaUp, double lambdaDown);

        // get entry of Matrix[col][row]
        Vec& operator[](const int idx);

        // get entry of Matrix[{row, col}]
        double& operator[](const pair<int, int>);

        Matrix operator[](const pair<pair<int, int>, pair<int, int>>);
        Matrix setSlice(pair<pair<int, int>, pair<int, int>>, Matrix&);
        Matrix getSlice(pair<pair<int, int>, pair<int, int>>);

        Matrix operator+(Matrix &);
        Matrix operator+=(Matrix &);
        Matrix operator+(double);
        Matrix operator+=(double);

        Matrix operator-(Matrix &);
        Matrix operator-(double);

        Matrix operator*(double);
        Matrix operator*=(double);

        // Matrix multiplication
        Matrix operator*(Matrix& b);
        Matrix elementwiseMul(Matrix &b);
        friend ostream& operator<<(ostream& os, Matrix a);

        Matrix swapRow(int idx1, int idx2);

    protected:
        vector<Vec> colVecArray;
        int nRow, nCol;
    };

}
#endif //CODES_MATRIX_H