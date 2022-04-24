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

	void init(const Matrix& P, const Matrix& xe, const Matrix& F);
	void extrapolateState(const Matrix& xe, const Matrix& F);
	void updateState(const Matrix& H, const Matrix& z, const Matrix& out);

	Matrix getState() const;
	Matrix getCovariance() const;

protected:
	Matrix _q;
	Matrix _p;
	Matrix _r;
	Matrix _x;
};


#endif //FILTERS_EKF_H
