//
// Created by 李喆昊 on 2020/9/29.
//
#include "../include/Matrix.h"
#include <cmath>

using namespace ReNLA;


Matrix::Matrix(vector<Vec> v) : colVecArray(v) {
    assert(!v.empty());
    nCol = v.size();
    nRow = v[0].size();
}

Matrix::Matrix(const Vec &v) : colVecArray({v}) {
    nCol = 1;
    nRow = v.size();
}

Matrix::Matrix(vector<vector<long double> > v, bool isColPrior) {
    assert(!v.empty());
    assert(!v[0].empty());
    if (isColPrior) {
        nCol = v.size();
        nRow = v[0].size();
        for (int i = 0; i < nCol; i++) {
            colVecArray.emplace_back(v[i]);
        }
    } else {
        nRow = v.size();
        nCol = v[0].size();
        for (int i = 0; i < nCol; i++) {
            vector<long double> colVec;
            for (int j = 0; j < nRow; j++) {
                colVec.push_back(v[j][i]);
            }
            colVecArray.emplace_back(colVec);
        }
    }
}

Matrix::Matrix(vector<long double> v) {
    assert(!v.empty());
    colVecArray.emplace_back(v);
    nCol = 1;
    nRow = v.size();
}

Matrix::Matrix(int nRow, int nCol, long double num) {
    assert(nRow > 0);
    assert(nCol > 0);
    vector<Vec> mat;
    for (int i = 0; i < nCol; i++) {
        mat.emplace_back(vector<long double>(nRow, num));
    }
    colVecArray = mat;
    this->nRow = nRow;
    this->nCol = nCol;
}

Matrix::Matrix(int nRow, int nCol) : Matrix::Matrix{nRow, nCol, 0.0} {}

Matrix::Matrix(int nRow) : Matrix::Matrix{nRow, nRow} {}

vector<int> Matrix::shape() const {
    return {nRow, nCol};
}

int Matrix::rowNum() const {
    return nRow;
}

int Matrix::colNum() const {
    return nCol;
}

Matrix Matrix::transpose() const {
    vector<vector<long double>> mat(nCol);
    for (int i = 0; i < nRow; i++) {
        for (int j = 0; j < nCol; j++) {
            mat[j].push_back((*this)[{i, j}]);
        }
    }
    return Matrix(mat);
}

Matrix Matrix::setNum(long double num) {
    for (int i = 0; i < nRow; i++) {
        for (int j = 0; j < nCol; j++)
            // or (*this)[{i,j}] = num;
            colVecArray[j][i] = num;
    }
    return (*this);
}

Matrix Matrix::setDiagValue(const vector<long double> v) {
    assert(v.size() == this->rowNum());
    for(int i = 0; i < this->rowNum(); i++)
    {
        (*this)[{i,i}] = v[i];
    }
    return (*this);
}

Matrix Matrix::setZero() {
    return this->setNum(0.0);
}

Matrix Matrix::setIdentity() {
    // Not require square
    this->setZero();
    for (int i = 0; i < nRow; i++) {
        this->colVecArray[i][i] = 1.0;
    }
    return (*this);
}

Vec &Matrix::operator[](const int idx) {
    return colVecArray[idx];
}

Vec Matrix::operator[](const int idx) const {
    return colVecArray[idx];
}

long double &Matrix::operator[](const pair<int, int> idx) {
    assert(idx.second >= 0 && idx.second < nCol && idx.first >= 0 && idx.first < nRow);
    return colVecArray[idx.second][idx.first];
}

long double Matrix::operator[](const pair<int, int> idx) const {
    assert(idx.second >= 0 && idx.second < nCol && idx.first >= 0 && idx.first < nRow);
    return colVecArray[idx.second][idx.first];
}

Vec Matrix::operator[](const pair<int, pair<int, int>> idx) const {
    int rowIdx = idx.first;
    auto colSlice = idx.second;
    assert(colSlice.second >= colSlice.first && colSlice.first >= 0 && colSlice.second <= nCol);
    vector<long double> v;
    for (int i = colSlice.first; i < colSlice.second; i++) {
        v.push_back((*this)[{rowIdx, i}]);
    }
    return Vec(v);
}

Vec Matrix::operator[](const pair<pair<int, int>, int> idx) const {
    int colIdx = idx.second;
    auto rowSlice = idx.first;
    return (*this)[colIdx][{rowSlice.first, rowSlice.second}];
}

Matrix Matrix::operator[](const pair<pair<int, int>, pair<int, int> > slice) const {
    auto rowSlice = slice.first;
    auto colSlice = slice.second;
    assert(colSlice.second >= colSlice.first && colSlice.first >= 0 && colSlice.second <= nCol);
    vector<Vec> m;
    for (int i = colSlice.first; i < colSlice.second; i++) {
        m.push_back((*this)[i][{rowSlice.first, rowSlice.second}]);
    }
    return Matrix(m);
}

Matrix Matrix::getSlice(const pair<pair<int, int>, pair<int, int>> slice) const {
    return (*this)[slice];
}

bool ReNLA::operator==(const Matrix &a, const Matrix &b) {
    assert(typeid(a) == typeid(Matrix));
    assert(typeid(b) == typeid(Matrix));
    if (a.nRow != b.nRow) return false;
    if (a.nCol != b.nCol) return false;
    for (int i = 0; i < a.nRow; i++) {
        for (int j = 0; j < a.nCol; j++) {
            if (fabs(a[{i, j}] - b[{i, j}]) > eps)
                return false;
        }
    }
    return true;
}

Matrix Matrix::setSlice(const pair<pair<int, int>, pair<int, int>> slice, const Matrix &A) {
    auto rowSlice = slice.first;
    auto colSlice = slice.second;
    assert(rowSlice.second >= rowSlice.first && rowSlice.first >= 0 && rowSlice.second <= nRow);
    assert(colSlice.second >= colSlice.first && colSlice.first >= 0 && colSlice.second <= nCol);
    assert((colSlice.second - colSlice.first) == A.nCol);
    assert((rowSlice.second - rowSlice.first) == A.nRow);

    auto rowStart = rowSlice.first;
    auto rowEnd = rowSlice.second;
    auto colStart = colSlice.first;
    auto colEnd = colSlice.second;
    for (int i = rowStart; i < rowEnd; i++) {
        for (int j = colStart; j < colEnd; j++) {
            (*this)[{i, j}] = A[{i - rowStart, j - colStart}];
        }
    }
    return (*this);
}

Matrix Matrix::addToSlice(const pair<pair<int, int>, pair<int, int> > slice, const Matrix & A) {
    return (*this).setSlice(slice, (*this)[slice] + A);
}

Matrix ReNLA::operator+(const Matrix &a, const Matrix &b) {
    assert(a.nRow == b.nRow);
    assert(a.nCol == b.nCol);
    Matrix m = Matrix(a.nRow, a.nCol).setZero();
    for (int i = 0; i < a.nRow; i++) {
        for (int j = 0; j < a.nCol; j++) {
            m[{i, j}] = a[{i, j}] + b[{i, j}];
        }
    }
    return m;
}

Matrix ReNLA::operator+(const Matrix &a, const long double b) {
    Matrix m = Matrix(a.nRow, a.nCol).setZero();
    for (int i = 0; i < a.nRow; i++) {
        for (int j = 0; j < a.nCol; j++) {
            m[{i, j}] = a[{i, j}] + b;
        }
    }
    return m;
}

Matrix ReNLA::operator+(const long double b, const Matrix &a) {
    return a + b;
}

Matrix ReNLA::operator-(const Matrix &a, const Matrix &b) {
    return a + (-b);
}

Matrix ReNLA::operator-(const Matrix &a, const long double b) {
    return a + (-b);
}

Matrix ReNLA::operator*(const Matrix &a, const Matrix &b) {
    assert(a.nCol == b.nRow);
    Matrix m = Matrix(a.nRow, b.nCol).setZero();

    for (int i = 0; i < m.nRow; i++) {
        for (int j = 0; j < m.nCol; j++) {
            for (int k = 0; k < a.nCol; k++) {
                m[{i, j}] += a[{i, k}] * b[{k, j}];
            }
        }
    }
    return m;
}

Matrix ReNLA::matMul(const Matrix &a, const Matrix &b) {
    return a * b;
}

Matrix ReNLA::operator*(const Matrix &a, const long double b) {
    Matrix m = Matrix(a.nRow, a.nCol).setZero();
    for (int i = 0; i < m.nRow; i++) {
        for (int j = 0; j < m.nCol; j++) {
            m[{i, j}] = a[{i, j}] * b;
        }
    }
    return m;
}

Matrix ReNLA::operator*(const long double b, const Matrix &a) {
    return a * b;
}

Vec ReNLA::operator*(const Matrix &a, const Vec &b) {
    assert(a.nCol == b.size());
    Vec v(a.nRow);
    for (int i = 0; i < a.nRow; i++) {
        for (int j = 0; j < a.nCol; j++) {
            v[i] += a[{i, j}] * b[j];
        }
    }
    return v;
}

//Vec ReNLA::operator* (const Vec& b, const Matrix& a)
//{
//    assert(b.size() == a.nRow);
//    //TODO: do we need to give Vec shape (1, n) or shape (n, 1) ?
//
//
//}

Matrix ReNLA::operator/(const Matrix &a, const long double b) {
    assert(fabs(b - 0.0) > eps);
    Matrix m = Matrix(a.nRow, a.nCol).setZero();
    for (int i = 0; i < m.nRow; i++) {
        for (int j = 0; j < m.nCol; j++) {
            m[{i, j}] = a[{i, j}] / b;
        }
    }
    return m;
}

Matrix Matrix::operator-() const {
    Matrix m = Matrix(this->nRow, this->nCol).setZero();
    for (int i = 0; i < m.nRow; i++) {
        for (int j = 0; j < m.nCol; j++) {
            m[{i, j}] = -(*this)[{i, j}];
        }
    }
    return m;
}

Matrix &Matrix::operator/=(const long double b) {
    (*this) = (*this) / b;
    return (*this);
}

Matrix &Matrix::operator+=(const Matrix &a) {
    (*this) = (*this) + a;
    return (*this);
}

Matrix &Matrix::operator+=(const long double a) {
    (*this) = (*this) + a;
    return (*this);
}

Matrix &Matrix::operator-=(const Matrix &a) {
    (*this) = (*this) - a;
    return (*this);
}

Matrix &Matrix::operator-=(const long double a) {
    (*this) = (*this) - a;
    return (*this);
}

Matrix &Matrix::operator*=(const Matrix &b) {
    (*this) = (*this) * b;
    return (*this);
}

//Vec Matrix::operator*=(const Vec& b) {
//    Vec v = (*this) * b;
//    return v;
//}

Matrix &Matrix::operator*=(const long double a) {
    (*this) = (*this) * a;
    return (*this);
}

Matrix Matrix::setTripleDiag(long double lambda, long double lambdaUp, long double lambdaDown) {
    assert(this->nRow == this->nCol);
    this->setZero();
    for (int i = 0; i < nRow; i++) {
        (*this)[{i, i}] = lambda;
        if (i >= 1)
            (*this)[{i, i - 1}] = lambdaDown;
        if (i <= nRow - 2)
            (*this)[{i, i + 1}] = lambdaUp;
    }
    return (*this);
}

Matrix Matrix::setHilbert() {
    for (int i = 0; i < nRow; i++) {
        for (int j = 0; j < nCol; j++) {
            (*this)[{i, j}] = 1.0 / ((i + 1) + (j + 1) - 1);
        }
    }
    return (*this);
}


ostream &ReNLA::operator<<(ostream &os, const Matrix a) {
    os << "[";
    for (int i = 0; i < a.nRow; i++) {
        os << "[ ";
        for (int j = 0; j < a.nCol; j++) {
            if(fabs(a[{i, j}]) <= 1e-15)
                os << 0 << "    ";
            else
                os << a[{i, j}];
            if (j < a.nCol - 1) os << ", ";
        }
        os << " ]";
        if (i < a.nRow - 1) os << "," << endl << " ";
    }
    os << "]" << endl;
    return os;
}

Matrix Matrix::swapRow(int idx1, int idx2) {
    assert(idx1 >= 0 && idx1 < nRow && idx2 >= 0 && idx2 < nRow);
    if (idx1 != idx2) {
        for (int i = 0; i < nCol; i++) {
            (*this)[i].swap(idx1, idx2);
        }
    }
    return (*this);
}

long double Matrix::normInfin() const {
    long double maxN = 0.0;
    for (int i = 0; i < this->nRow; i++) {
        Vec r = (*this)[{i, {0, this->nCol}}];
        long double rN = r.norm1();
        if (rN > maxN)
            maxN = rN;
    }
    return maxN;
}

long double Matrix::norm1() const {
    // TODO
}

long double Matrix::norm2() const {
    // TODO
}

Matrix Matrix::inv() const {
    //TODO chap1 execises
}

long double Matrix::normFrobenius() const {
    long double norm = 0.0;
    for(int i = 0; i < this->rowNum(); i++)
    {
        for(int j = 0; j < this->colNum(); j++)
        {
            norm += (*this)[j][i] * (*this)[j][i];
        }
    }
    return sqrt(norm);
}

long double Matrix::normNonDiag() const {
    long double norm = 0.0;
    for(int i = 0; i < this->rowNum(); i++)
    {
        for(int j = 0; j < this->colNum(); j++)
        {
            if(i != j)
                norm += (*this)[j][i] * (*this)[j][i];
        }
    }
    return sqrt(norm);
}

Matrix Matrix::lowTriangleInv() const
{
    // Assert A is a low triangle matrix
    assert(this->rowNum() == this->colNum()); // Not support pseudo-inverse now
    int n = this->rowNum();

    auto Ainv=Matrix(n).setIdentity();

    long double det = 1.0;
    for (int i = 0; i < n; i++) {
        det *= (*this)[{i, i}];
    }
    assert(det != 0);

    for (int i = n - 1; i >= 0; i--) {
        auto v = Ainv[{{i, n}, i}];

        for (int j = i-1; j >=0; j--)
        {
            Ainv[j].addToSlice(i, n, v * (- (*this)[{i, j}] / (*this)[{i, i}]));
        }
        Ainv[i].mulToSlice(i,n, 1.0 / (*this)[{i,i}]);
    }
    return Ainv;
}

vector<long double> Matrix::getDiagValue() const {
    // TODO: need to check square matrix?
    vector<long double> diags(this->rowNum());
    for(int i = 0; i < this->rowNum(); i++)
    {
        diags[i] = (*this)[{i,i}];
    }
    return diags;
}

Matrix Matrix::getUpTriangle() const {
    // TODO: need to check square matrix ?
    auto A = (*this);

    for(int i = 0; i < A.rowNum(); i++)
    {
        for(int j = 0; j <= i; j++)
        {
            // set low triangle to zeros
            A[{i, j}] = 0.0;
        }
    }
    return A;
}
Matrix Matrix::getLowTriangle() const {
    // TODO: need to check square matrix ?
    auto A = (*this);
    for(int i = 0; i < A.rowNum(); i++)
    {
        for(int j = i; j < A.colNum(); j++)
        {
            // set up triangle to zeros
            A[{i, j}] = 0.0;
        }
    }
    return A;
}