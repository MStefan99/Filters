//
// Created by mikha on 11.1.22.
//

#ifndef FILTERS_KALMAN_HPP
#define FILTERS_KALMAN_HPP

#include "Matrix.hpp"


class Kalman {
public:
	Kalman() = delete;
	Kalman(const Kalman& kalman) = default;
	Kalman(const Matrix& Q, const Matrix& R);

	void init(const Matrix& F, const Matrix& P, const Matrix& x);
	void predict(const Matrix& F);
	void correct(const Matrix& z, const Matrix& H);

	Matrix getState() const;
	Matrix getCovariance() const;

protected:
	Matrix _q;
	Matrix _p;
	Matrix _r;
	Matrix _x;
};


#endif //FILTERS_KALMAN_HPP
