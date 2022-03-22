//
// Created by MStefan99 on 21.3.22.
//

#ifndef FILTERS_EKF_H
#define FILTERS_EKF_H


#include "Matrix.h"


template <size xLen, size zLen, size uLen>
class EKF {
public:
	EKF() = delete;
	EKF(const EKF& ekf) = default;
	EKF(const Matrix<xLen, xLen>& Q, const Matrix<zLen, zLen>& R);

	void init(const Matrix<xLen, xLen>& F, const Matrix<xLen, xLen>& P, const Vector<xLen>& x, const Vector<uLen>& u);
	void extrapolateState(const Matrix<xLen, xLen>& F, const Vector<uLen>& u);
	void updateState(const Vector<zLen>& z, const Matrix<zLen, xLen>& H);

	void setCallbacks(
			Vector<xLen> (* extrapolateState)(const Vector<xLen>& x, const Vector<uLen>& u),
			Vector<zLen> (* updateState)(const Vector<xLen>& x)
	);

	Vector<xLen> getState() const;
	Matrix<xLen, xLen> getCovariance() const;

protected:
	Matrix<xLen, xLen> _q;
	Matrix<xLen, xLen> _p;
	Matrix<zLen, zLen> _r;
	Vector<xLen> _x;

	Vector<xLen> (* _extrapolateState)(const Vector<xLen>& x, const Vector<uLen>& u) {nullptr};
	Vector<zLen> (* _updateState)(const Vector<xLen>& x) {nullptr};
};


template <size xLen, size zLen, size uLen>
EKF<xLen,zLen,uLen>::EKF(const Matrix<xLen,xLen>& Q, const Matrix<zLen,zLen>& R):
		_q {Q}, _r {R} {
	// Nothing to do
}


template <size xLen, size zLen, size uLen>
void EKF<xLen,zLen,uLen>::init(const Matrix<xLen,xLen>& F, const Matrix<xLen,xLen>& P,
		const Vector<xLen>& x, const Vector<uLen>& u) {
	_x = _extrapolateState(x, u);
	_p = F * P * F.transpose() + _q;
}


template <size xLen, size zLen, size uLen>
void EKF<xLen,zLen,uLen>::extrapolateState(const Matrix<xLen, xLen>& F, const Vector<uLen>& u) {
	_x = _extrapolateState(_x, u);
	_p = F * _p * F.transpose() + _q;
}


template <size xLen, size zLen, size uLen>
void EKF<xLen,zLen,uLen>::updateState(const Vector<zLen>& z, const Matrix<zLen, xLen>& H) {
	Matrix<xLen,zLen> K {_p * H.transpose() * (H * _p * H.transpose() + _r).invert()};
	_x = _x + K * (z - _updateState(_x));

	Matrix<xLen,xLen> temp {Matrix<0,0>::identity<xLen>() - K * H};
	_p = temp * _p;
}


template <size xLen, size zLen, size uLen>
void EKF<xLen,zLen,uLen>::setCallbacks(
		Vector<xLen> (* extrapolateState)(const Vector<xLen>& x, const Vector<uLen>& u),
		Vector<zLen> (* updateState)(const Vector<xLen>& x)
) {
	_extrapolateState = extrapolateState;
	_updateState = updateState;
}


template <size xLen, size zLen, size uLen>
Vector<xLen> EKF<xLen,zLen,uLen>::getState() const {
	return _x;
}


template <size xLen, size zLen, size uLen>
Matrix<xLen,xLen> EKF<xLen,zLen,uLen>::getCovariance() const {
	return _p;
}


#endif //FILTERS_EKF_H
