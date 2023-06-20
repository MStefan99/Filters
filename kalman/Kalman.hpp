//
// Created by mikha on 11.1.22.
//

#ifndef FILTERS_KALMAN_HPP
#define FILTERS_KALMAN_HPP

#include "FixMath.hpp"
#include "Matrix.hpp"


template <class scalar = fixed, class size_type = unsigned int>
class Kalman {
public:
	Kalman() = delete;
	Kalman(const Kalman& kalman) = default;
	Kalman(const Matrix<scalar, size_type>& Q, const Matrix<scalar, size_type>& R);

	void init(const Matrix<scalar, size_type>& F, const Matrix<scalar, size_type>& P, const Matrix<scalar, size_type>& x);
	void predict(const Matrix<scalar, size_type>& F);
	void correct(const Matrix<scalar, size_type>& z, const Matrix<scalar, size_type>& H);

	Matrix<scalar, size_type> getState() const;
	Matrix<scalar, size_type> getCovariance() const;

protected:
	Matrix<scalar, size_type> _q;
	Matrix<scalar, size_type> _p;
	Matrix<scalar, size_type> _r;
	Matrix<scalar, size_type> _x;
};

template <class scalar, class size_type>
Kalman<scalar, size_type>::Kalman(const Matrix<scalar, size_type>& Q, const Matrix<scalar, size_type>& R):
		_q {Q}, _r {R}, _x {1, Q.getHeight()} {
	// Nothing to do
}


template <class scalar, class size_type>
void Kalman<scalar, size_type>::init(const Matrix<scalar, size_type>& F, const Matrix<scalar, size_type>& P, const Matrix<scalar, size_type>& x) {
	_x = F * x;
	_p = F * P * F.transpose() + _q;
}


template <class scalar, class size_type>
void Kalman<scalar, size_type>::predict(const Matrix<scalar, size_type>& F) {
	_x = F * _x;
	_p = F * _p * F.transpose() + _q;
}


template <class scalar, class size_type>
void Kalman<scalar, size_type>::correct(const Matrix<scalar, size_type>& z, const Matrix<scalar, size_type>& H) {
	Matrix K {_p * H.transpose() * (H * _p * H.transpose() + _r).invert()};
	_x = _x + K * (z - H * _x);

	Matrix temp {K * H};
	temp = Matrix<scalar, size_type>::identity(temp.getHeight()) - temp;
	_p = temp * _p * temp.transpose() + K * _r * K.transpose();
}


template <class scalar, class size_type>
Matrix <scalar, size_type>Kalman<scalar, size_type>::getState() const {
	return _x;
}


template <class scalar, class size_type>
Matrix <scalar, size_type>Kalman<scalar, size_type>::getCovariance() const {
	return _p;
}

#endif //FILTERS_KALMAN_HPP
