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
	EKF(const Matrix& Q, const Matrix& R);

	void init(const Matrix& F, const Matrix& P, const Matrix& x, const Matrix& u);
	void extrapolateState(const Matrix& F, const Matrix& u);
	void updateState(const Matrix& z, const Matrix& H);

	void setCallbacks(
			Matrix (* extrapolateState)(const Matrix& x, const Matrix& u),
			Matrix (* updateState)(const Matrix& x)
	);

	Matrix getState() const;
	Matrix getCovariance() const;

protected:
	Matrix _q;
	Matrix _p;
	Matrix _r;
	Matrix _x;

	Matrix (* _extrapolateState)(const Matrix& x, const Matrix& u) {nullptr};
	Matrix (* _updateState)(const Matrix& x) {nullptr};
};


#endif //FILTERS_EKF_H
