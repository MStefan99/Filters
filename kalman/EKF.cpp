//
// Created by MStefan99 on 29.3.22.
//

#include "EKF.h"


EKF::EKF(const Matrix& Q, const Matrix& R,
		Matrix (* extrapolateState)(const Matrix& x, const Matrix& u, float dt),
		Matrix (* getOutput)(const Matrix& x),
		Matrix (* getF)(const Matrix& x, const Matrix& u, float dt),
		Matrix (* getH)(const Matrix& x)):
		_q {Q}, _r {R},
		_extrapolateState {extrapolateState},
		_getOutput {getOutput},
		_getF {getF}, _getH {getH} {
	// Nothing to do
}


void EKF::init(const Matrix& P,
		const Matrix& x, const Matrix& u) {
	Matrix F {_getF(x, u, 1)};

	_x = _extrapolateState(x, u, 1);
	_p = F * P * F.transpose() + _q;
}


void EKF::extrapolateState(const Matrix& u, float dt) {
	Matrix F {_getF(_x, u, dt)};

	_x = _extrapolateState(_x, u, dt);
	_p = F * _p * F.transpose() + _q;
}


void EKF::updateState(const Matrix& z) {
	Matrix H {_getH(_x)};

	Matrix K {_p * H.transpose() * (H * _p * H.transpose() + _r).invert()};
	_x = _x + K * (z - _getOutput(_x));

	Matrix temp {Matrix::identity(_x.getHeight()) - K * H};
	_p = temp * _p;
}


Matrix EKF::getState() const {
	return _x;
}


Matrix EKF::getCovariance() const {
	return _p;
}