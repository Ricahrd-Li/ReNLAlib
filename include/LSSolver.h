//
// Created by 李喆昊 on 2020/11/12.
//

#ifndef CODES_LSSOLVER_H
#define CODES_LSSOLVER_H

#include "Matrix.h"
#include "Vec.h"
#include "LinearSolver.h"

namespace ReNLA
{
    class LSSolver{
    public:
        LSSolver(const Matrix& A, const Vec& b);

        //@brief: do householder transform on x
        //@return: Vec v. beta stored at v[0] (v[0] should be 1)
        static Vec householder(Vec x); // \beta can be stored at returned Vec[0]

        //@brief: do givens transform on [..., a, ..., b,...]
        //@return: pair{cos, sin} to make b'=0 after transform
        static pair<long double, long double> givens(long double a, long double b);
        static pair<Matrix, Vec> QRdecomposition(Matrix A);
        Vec QRSolve();

    private:
        Matrix A;
        Vec b;
    };
}
#endif //CODES_LSSOLVER_H
