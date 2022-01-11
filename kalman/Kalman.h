//
// Created by mikha on 11.1.22.
//

#ifndef FILTERS_KALMAN_H
#define FILTERS_KALMAN_H

#include "Matrix.h"


class Kalman {
public:
	Kalman() = delete;
	Kalman(const Kalman& kalman) = default;
	Kalman(const Matrix& F, const Matrix& Q, const Matrix& R);

	void init(const Matrix& X, const Matrix& P);
	void predict();  // TODO: implement variable dt
	void correct(const Matrix& Z, const Matrix& H);

	Matrix getState() const;
	Matrix getCovariance() const;

protected:
	Matrix _f;
	Matrix _q;
	Matrix _r;
	Matrix _x;
	Matrix _p;
};

#endif //FILTERS_KALMAN_H
