//
// Created by MStefan99 on 29.3.22.
//

#include "EKF.h"


EKF::EKF(const Matrix& Q, const Matrix& R):
		_q {Q}, _r {R} {
	// Nothing to do
}


void EKF::init(const Matrix& F, const Matrix& P,
		const Matrix& x, const Matrix& u) {
	_x = _extrapolateState(x, u);
	_p = F * P * F.transpose() + _q;
}


void EKF::extrapolateState(const Matrix& F, const Matrix& u) {
	_x = _extrapolateState(_x, u);
	_p = F * _p * F.transpose() + _q;
}


void EKF::updateState(const Matrix& z, const Matrix& H) {
	Matrix K {_p * H.transpose() * (H * _p * H.transpose() + _r).invert()};
	_x = _x + K * (z - _updateState(_x));

	Matrix temp {Matrix::identity(_x.getHeight()) - K * H};
	_p = temp * _p;
}


void EKF::setCallbacks(
		Matrix (* extrapolateState)(const Matrix& x, const Matrix& u),
		Matrix (* updateState)(const Matrix& x)
) {
	_extrapolateState = extrapolateState;
	_updateState = updateState;
}


Matrix EKF::getState() const {
	return _x;
}


Matrix EKF::getCovariance() const {
	return _p;
}