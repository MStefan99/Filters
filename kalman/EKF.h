//
// Created by MStefan99 on 21.3.22.
//

#ifndef FILTERS_EKF_H
#define FILTERS_EKF_H


#include "Matrix.h"


class EKF {
public:
	EKF() = delete;
	EKF(const EKF& ekf) = default;
	EKF(const Matrix& Q, const Matrix& R,
			Matrix (* extrapolateState)(const Matrix& x, const Matrix& u, float dt),
			Matrix (* getOutput)(const Matrix& x),
			Matrix (* getF)(const Matrix& x, const Matrix& u, float dt),
			Matrix (* getH)(const Matrix& x));

	void init(const Matrix& P, const Matrix& x, const Matrix& u);
	void extrapolateState(const Matrix& u, float dt);
	void updateState(const Matrix& z);

	Matrix getState() const;
	Matrix getCovariance() const;

protected:
	Matrix _q;
	Matrix _p;
	Matrix _r;
	Matrix _x;

	Matrix (* _extrapolateState)(const Matrix& x, const Matrix& u, float dt) {nullptr};  // f(x, u)
	Matrix (* _getOutput)(const Matrix& x) {nullptr};  // h(x)

	Matrix (* _getF)(const Matrix& x, const Matrix& u, float dt) {nullptr};  // Fj(x, u)
	Matrix (* _getH)(const Matrix& x) {nullptr};  // Hj(x)
};


#endif //FILTERS_EKF_H
