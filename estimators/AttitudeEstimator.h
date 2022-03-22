//
// Created by MStefan99 on 21.3.22.
//

#ifndef FILTERS_ATTITUDEESTIMATOR_H
#define FILTERS_ATTITUDEESTIMATOR_H

#include "EKF.h"


#define xLen 2
#define zLen 3
#define uLen 2


class AttitudeEstimator {
public:
	explicit AttitudeEstimator(scalar accUncertainty);
	void init(float roll, float pitch);

	void update(scalar dt);
	void measure(scalar dt,
			const TL::vector<scalar>& rot,
			const TL::vector<scalar>& acc);

	scalar getPitch() const;
	scalar getRoll() const;

protected:
	Matrix<xLen,xLen> F(Vector<xLen> x, Vector<uLen> u);
	Matrix<zLen,xLen> H(Vector<xLen> x);

	EKF<xLen,zLen,uLen> _k;
};


#endif //FILTERS_ATTITUDEESTIMATOR_H
