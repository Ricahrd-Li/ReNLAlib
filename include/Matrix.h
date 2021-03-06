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

namespace ReNLA {
    class Matrix {
    public:
        // Create Matrix from vector<vector>,
        // if isColPrior, then every vector in v is a column vector, otherwise row vector
        Matrix(vector<vector<long double>> v, bool isColPrior = false);

        // Create Matrix from vector<Vec>
        Matrix(vector<Vec>);

        // Create a one column matrix from Vec
        Matrix(const Vec &);

        // Create a one column matrix from vector
        Matrix(vector<long double>);

        // Create a nRow x nCol matrix with all entries set as ${num};
        Matrix(int nRow, int nCol, long double num);

        // Create a zero matrix of shape ${nRow} x ${nCol}
        Matrix(int nRow, int nCol);

        // Create a square matrix of shape nRow x nRow
        Matrix(int nRow);

        // Return a vector<int> of the shape of matrix
        vector<int> shape() const;

        // Return num of rows
        virtual int rowNum() const;

        // Return num of cols
        virtual int colNum() const;

        Matrix transpose() const;

        // set all entries of matrix to ${num}
        Matrix setNum(long double num);

        // set the diagonal entires of matrix to vector<long double>
        Matrix setDiagValue(const vector<long double>);

        Matrix setIdentity();

        // set entries to be all zeros
        Matrix setZero();

        // set matrix to be a triple diagonal matrix
        Matrix setTripleDiag(long double lambda, long double lambdaUp, long double lambdaDown);

        Matrix setHilbert();

        // get entry of Matrix[col][row]
        Vec &operator[](const int idx);

        Vec operator[](const int idx) const;

        // get entry of Matrix[{row, col}]
        long double &operator[](const pair<int, int>);

        long double operator[](const pair<int, int>) const;

        // slice row vector
        Vec operator[](const pair<int, pair<int, int>>) const;

        // slice col vector
        Vec operator[](const pair<pair<int, int>, int>) const;

        // slice sub matrix
        Matrix operator[](const pair<pair<int, int>, pair<int, int>>) const;

        Matrix setSlice(const pair<pair<int, int>, pair<int, int>>, const Matrix &);

        Matrix getSlice(const pair<pair<int, int>, pair<int, int>>) const;

        Matrix addToSlice(const pair<pair<int, int>, pair<int, int>>, const Matrix &);

        friend ostream &operator<<(ostream &os, const Matrix a);

        friend bool operator==(const Matrix &a, const Matrix &b);

        friend Matrix operator+(const Matrix &a, const Matrix &b);

        friend Matrix operator+(const Matrix &a, const long double b);

        friend Matrix operator+(const long double b, const Matrix &a);

        friend Matrix operator-(const Matrix &a, const Matrix &b);

        friend Matrix operator-(const Matrix &a, const long double b);

        friend Matrix operator*(const Matrix &a, const Matrix &b);

        friend Matrix matMul(const Matrix &a, const Matrix &b);

        friend Vec operator*(const Matrix &a, const Vec &v);

        //TODO: friend Vec operator* (const Vec& v, const Matrix& a);
        friend Matrix operator*(const Matrix &a, const long double b);

        friend Matrix operator*(const long double b, const Matrix &a);

        friend Matrix operator/(const Matrix &a, const long double b);

        Matrix operator-() const;

        Matrix &operator+=(const Matrix &);

        Matrix &operator+=(const long double);

        Matrix &operator-=(const Matrix &);

        Matrix &operator-=(const long double);

        // Matrix multiplication
        Matrix &operator*=(const Matrix &b);

        Matrix &operator*=(const long double);
        //TODO: Vec operator*=(const Vec& b);

        Matrix &operator/=(const long double);

        Matrix &elementwiseMul(Matrix &b);

        Matrix swapRow(int idx1, int idx2);

        long double normInfin() const;

        long double norm1() const;

        long double norm2() const;

        // get Frobenius norm of matrix
        long double normFrobenius() const;

        // get non diagonal norm of matrix: E(A) = \sqrt(||A||_F^2 - \Sum a_{ii}^2)
        long double normNonDiag() const;

        // inverse a matrix
        Matrix inv() const;

        Matrix lowTriangleInv() const;

        vector<long double> getDiagValue() const;

        Matrix getLowTriangle() const;

        Matrix getUpTriangle() const;

    protected:
        vector<Vec> colVecArray;
        int nRow, nCol;
    };
}
#endif //CODES_MATRIX_H
