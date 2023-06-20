//
// Created by MStefan99 on 21.3.22.
//

#ifndef FILTERS_EKF_HPP
#define FILTERS_EKF_HPP


#include "Matrix.hpp"
#include "FixMath.hpp"


template <class scalar = fixed, class size_type = unsigned int>
class EKF {
public:
	EKF() = delete;
	EKF(const EKF& ekf) = default;
	EKF(const Matrix<scalar, size_type>& Q, const Matrix<scalar, size_type>& R);

	void init(const Matrix<scalar, size_type>& P, const Matrix<scalar, size_type>& xe, const Matrix<scalar, size_type>& F);
	void extrapolateState(const Matrix<scalar, size_type>& xe, const Matrix<scalar, size_type>& F);
	void updateState(const Matrix<scalar, size_type>& H, const Matrix<scalar, size_type>& z, const Matrix<scalar, size_type>& out);

	Matrix<scalar, size_type> getState() const;
	Matrix<scalar, size_type> getCovariance() const;

protected:
	Matrix<scalar, size_type> _q;
	Matrix<scalar, size_type> _p;
	Matrix<scalar, size_type> _r;
	Matrix<scalar, size_type> _x;
};

template <class scalar, class size_type>
EKF<scalar, size_type>::EKF(const Matrix<scalar, size_type>& Q, const Matrix<scalar, size_type>& R):
		_q {Q}, _r {R} {
	// Nothing to do
}


template <class scalar, class size_type>
void EKF<scalar, size_type>::init(const Matrix<scalar, size_type>& P,
		const Matrix<scalar, size_type>& xe,
		const Matrix<scalar, size_type>& F) {
	_x = xe;
	_p = F * P * F.transpose() + _q;
}


template <class scalar, class size_type>
void EKF<scalar, size_type>::extrapolateState(const Matrix<scalar, size_type>& xe, const Matrix<scalar, size_type>& F) {
	_x = xe;
	_p = F * _p * F.transpose() + _q;
}


template <class scalar, class size_type>
void EKF<scalar, size_type>::updateState(const Matrix<scalar, size_type>& H, const Matrix<scalar, size_type>& z, const Matrix<scalar, size_type>& out) {
	Matrix K {_p * H.transpose() * (H * _p * H.transpose() + _r).invert()};
	_x = _x + K * (z - out);

	Matrix temp {Matrix<scalar, size_type>::identity(_x.getHeight()) - K * H};
	_p = temp * _p;
}


template <class scalar, class size_type>
Matrix<scalar, size_type> EKF<scalar, size_type>::getState() const {
	return _x;
}


template <class scalar, class size_type>
Matrix<scalar, size_type> EKF<scalar, size_type>::getCovariance() const {
	return _p;
}

#endif //FILTERS_EKF_HPP
