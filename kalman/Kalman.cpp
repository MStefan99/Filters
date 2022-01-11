//
// Created by mikha on 11.1.22.
//

#include "Kalman.h"


Kalman::Kalman(const Matrix& F, const Matrix& Q, const Matrix& R):
_f {F}, _q {Q}, _r {R} {
	// Nothing to do
}


void Kalman::init(const Matrix& X, const Matrix& P) {
	_x = _f * X;
	_p = _f * P * _f.transpose() + _q;
}


void Kalman::predict() {
	_x = _f * _x;
	_p = _f * _p * _f.transpose() + _q;
}


void Kalman::correct(const Matrix& Z, const Matrix& H) {
	Matrix K {_p * H.transpose() * (H * _p * H.transpose() + _r).invert()};
	_x = _x + K * (Z - H * _x);

	Matrix temp {K * H};
	temp = Matrix::identity(temp.getHeight()) - temp;
	_p = temp * _p * temp.transpose() + K * _r * K.transpose();
}


Matrix Kalman::getState() const {
	return _x;
}


Matrix Kalman::getCovariance() const {
	return _p;
}
