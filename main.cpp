//
// Created by mikha on 2.1.22.
//

#include <iostream>

#include "FixMath.hpp"
#include "AttitudeEstimator.hpp"


int main() {
	uint32_t a = 5;
	fix_math::fixed b(a);
	b += 1;
	std::cout << static_cast<uint32_t>(b) << std::endl;

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
									{9.81f}
							},
							0.1f);

		std::cout << "Estimate for iteration " << i + 1 << std::endl;
		std::cout << "Pitch: " << e.getPitch() << ", roll: " << e.getRoll() << std::endl;
		std::cout << "Covariance: " << e.getUncertainty() << std::endl;
	}

	return 0;
}
