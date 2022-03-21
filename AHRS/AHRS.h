//
// Created by mikha on 25.1.22.
//

// Attitude and heading reference system v0.1

#ifndef FILTERS_AHRS_H
#define FILTERS_AHRS_H

#include "Kalman.h"


class AHRS {
public:
	AHRS();

	void update(const TL::vector<scalar>& acc,
							const TL::vector<scalar>& rot);
	void update(const TL::vector<scalar>& acc,
							const TL::vector<scalar>& rot,
							const TL::vector<scalar>& mag);

	TL::vector<scalar> getLocation() const;
	TL::vector<scalar> getOrientation() const;

protected:
	Kalman _estimator;
};

#endif // FILTERS_AHRS_H
