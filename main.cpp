//
// Created by mikha on 2.1.22.
//

#include <iostream>

#include "FixMath.hpp"
#include "AttitudeEstimator.hpp"


int main() {
	AttitudeEstimator e{0.05f};

	e.init(0, 0);

	for (int i{0}; i < 10; ++i) {
		e.measure({
									{0},
									{0},
									{0}
							}, {
									{0},
									{0},
									{9.81}
							},
							0.1f);

		std::cout << "Estimate for iteration " << i + 1 << std::endl;
		std::cout << "Pitch: " << e.getPitch() << ", roll: " << e.getRoll() << std::endl;
		std::cout << "Covariance: " << e.getUncertainty() << std::endl;
	}

	return 0;
}
