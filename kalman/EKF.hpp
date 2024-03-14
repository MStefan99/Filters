//
// Created by MStefan99 on 21.3.22.
//

#ifndef FILTERS_EKF_HPP
#define FILTERS_EKF_HPP


#include "Matrix.hpp"


template <class scalar = float, class size_type = unsigned int, size_type nx = 1, size_type nu = 1, size_type nz = 1>
class EKF {
public:
	EKF() = delete;
	EKF(const EKF& ekf) = default;
	EKF(const Matrix<scalar, size_type, nx, 1>& x0,
	    const Matrix<scalar, size_type, nx, nx>& P0,
	    const Matrix<scalar, size_type, nx, nx>& Q,
	    const Matrix<scalar, size_type, nz, nz>& R);

	void predict(const Matrix<scalar, size_type, nx, nx>& F,
	             const Matrix<scalar, size_type, nx, 1>& xe);
	void correct(const Matrix<scalar, size_type, nz, nx>& H,
	             const Matrix<scalar, size_type, nz, 1>& z,
	             const Matrix<scalar, size_type, nz, 1>& out);

	Matrix<scalar, size_type, nx, 1> x() const;
	Matrix<scalar, size_type, nx, nx> P() const;

protected:
	Matrix<scalar, size_type, nx, 1> _x {};
	Matrix<scalar, size_type, nx, nx> _P {};
	Matrix<scalar, size_type, nx, nx> _Q {};
	Matrix<scalar, size_type, nz, nz> _R {};
};

template <class scalar, class size_type, size_type nx, size_type nu, size_type nz>
EKF<scalar, size_type, nx, nu, nz>::EKF(const Matrix<scalar, size_type, nx, 1>& x0,
                                        const Matrix<scalar, size_type, nx, nx>& P0,
                                        const Matrix<scalar, size_type, nx, nx>& Q,
                                        const Matrix<scalar, size_type, nz, nz>& R):
		_x {x0}, _P {P0}, _Q {Q}, _R {R} {
	// Nothing to do
}


template <class scalar, class size_type, size_type nx, size_type nu, size_type nz>
void EKF<scalar, size_type, nx, nu, nz>::predict(const Matrix<scalar, size_type, nx, nx>& F,
                                                 const Matrix<scalar, size_type, nx, 1>& xe) {
	_x = xe;
	_P = F * _P * F.transpose() + _Q;
}


template <class scalar, class size_type, size_type nx, size_type nu, size_type nz>
void EKF<scalar, size_type, nx, nu, nz>::correct(const Matrix<scalar, size_type, nz, nx>& H,
                                                 const Matrix<scalar, size_type, nz, 1>& z,
                                                 const Matrix<scalar, size_type, nz, 1>& out) {
	auto K {_P * H.transpose() * (H * _P * H.transpose() + _R).inverse()};
	_x = _x + K * (z - out);
	_P = Matrix<scalar, size_type, nx, nx>::identity() - K * H * _P;
}


template <class scalar, class size_type, size_type nx, size_type nu, size_type nz>
Matrix<scalar, size_type, nx, 1> EKF<scalar, size_type, nx, nu, nz>::x() const {
	return _x;
}


template <class scalar, class size_type, size_type nx, size_type nu, size_type nz>
Matrix<scalar, size_type, nx, nx> EKF<scalar, size_type, nx, nu, nz>::P() const {
	return _P;
}

#endif //FILTERS_EKF_HPP
