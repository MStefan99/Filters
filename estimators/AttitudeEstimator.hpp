//
// Created by MStefan99 on 21.3.22.
//

#ifndef FILTERS_ATTITUDEESTIMATOR_HPP
#define FILTERS_ATTITUDEESTIMATOR_HPP

#include <cmath>
#include "EKF.hpp"


class AttitudeEstimator {
public:
	explicit AttitudeEstimator(float accUncertainty);
	void init(float roll, float pitch);

	void update(float dt);
	void measure(
			const Matrix& rot,
			const Matrix& acc,
			float dt);

	float getRoll() const;
	float getPitch() const;

	Matrix getUncertainty() const;

protected:
	Matrix _u {};

	EKF _k;
};


#endif //FILTERS_ATTITUDEESTIMATOR_HPP
