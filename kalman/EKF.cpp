//
// Created by MStefan99 on 29.3.22.
//

#include "EKF.hpp"


EKF::EKF(const Matrix& Q, const Matrix& R):
		_q {Q}, _r {R} {
	// Nothing to do
}


void EKF::init(const Matrix& P,
		const Matrix& xe,
		const Matrix& F) {
	_x = xe;
	_p = F * P * F.transpose() + _q;
}


void EKF::extrapolateState(const Matrix& xe, const Matrix& F) {
	_x = xe;
	_p = F * _p * F.transpose() + _q;
}


void EKF::updateState(const Matrix& H, const Matrix& z, const Matrix& out) {
	Matrix K {_p * H.transpose() * (H * _p * H.transpose() + _r).invert()};
	_x = _x + K * (z - out);

	Matrix temp {Matrix::identity(_x.getHeight()) - K * H};
	_p = temp * _p;
}


Matrix EKF::getState() const {
	return _x;
}


Matrix EKF::getCovariance() const {
	return _p;
}