//
// Created by mikha on 11.1.22.
//

#include "Kalman.hpp"


Kalman::Kalman(const Matrix& Q, const Matrix& R):
		_q {Q}, _r {R}, _x {1, Q.getHeight()} {
	// Nothing to do
}


void Kalman::init(const Matrix& F, const Matrix& P, const Matrix& x) {
	_x = F * x;
	_p = F * P * F.transpose() + _q;
}


void Kalman::predict(const Matrix& F) {
	_x = F * _x;
	_p = F * _p * F.transpose() + _q;
}


void Kalman::correct(const Matrix& z, const Matrix& H) {
	Matrix K {_p * H.transpose() * (H * _p * H.transpose() + _r).invert()};
	_x = _x + K * (z - H * _x);

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
