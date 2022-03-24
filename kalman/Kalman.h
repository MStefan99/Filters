//
// Created by mikha on 11.1.22.
//

#ifndef FILTERS_KALMAN_H
#define FILTERS_KALMAN_H

#include "Matrix.h"


template <size xLen, size zLen>
class Kalman {
public:
	Kalman() = delete;
	Kalman(const Kalman& kalman) = default;
	Kalman(const Matrix<xLen, xLen>& Q, const Matrix<zLen, zLen>& R);

	void init(const Matrix<xLen, xLen>& F, const Matrix<xLen, xLen>& P, const Vector<xLen>& x);
	void predict(const Matrix<xLen, xLen>& F);
	void correct(const Vector<zLen>& z, const Matrix<zLen, xLen>& H);

	Vector<xLen> getState() const;
	Matrix<xLen, xLen> getCovariance() const;

protected:
	Matrix<xLen, xLen> _q;
	Matrix<xLen, xLen> _p;
	Matrix<zLen, zLen> _r;
	Vector<xLen> _x;
};


template <size xLen, size zLen>
Kalman<xLen, zLen>::Kalman(const Matrix<xLen, xLen>& Q, const Matrix<zLen, zLen>& R):
		_q {Q}, _r {R}, _x {1, xLen} {
	// Nothing to do
}


template <size xLen, size zLen>
void Kalman<xLen, zLen>::init(const Matrix<xLen, xLen>& F,
		const Matrix<xLen, xLen>& P, const Vector<xLen>& x) {
	_x = F * x;
	_p = F * P * F.transpose() + _q;
}


template <size xLen, size zLen>
void Kalman<xLen, zLen>::predict(const Matrix<xLen, xLen>& F) {
	_x = F * _x;
	_p = F * _p * F.transpose() + _q;
}


template <size xLen, size zLen>
void Kalman<xLen, zLen>::correct(const Vector<zLen>& Z, const Matrix<zLen, xLen>& H) {
	Matrix<xLen, zLen> K {_p * H.transpose() * (H * _p * H.transpose() + _r).invert()};
	_x = _x + K * (Z - H * _x);

	Matrix<xLen, xLen> temp {Matrix<0, 0>::identity<xLen>() - K * H};
	_p = temp * _p * temp.transpose() + K * _r * K.transpose();
}


template <size xLen, size zLen>
Vector<xLen> Kalman<xLen, zLen>::getState() const {
	return _x;
}


template <size xLen, size zLen>
Matrix<xLen, xLen> Kalman<xLen, zLen>::getCovariance() const {
	return _p;
}


#endif //FILTERS_KALMAN_H
