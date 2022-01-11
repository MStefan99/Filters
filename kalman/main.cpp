//
// Created by mikha on 2.1.22.
//

#include <iostream>
#include <vector>

#include "Kalman.h"


int main() {
	const float dt = 0.02;
	const float uncertainty = 0.0392;

	Kalman k {
			{
					{1, 0, 0, dt, 0, 0, dt * dt / 2, 0, 0},
					{0, 1, 0, 0, dt, 0, 0, dt * dt / 2, 0},
					{0, 0, 1, 0, 0, dt, 0, 0, dt * dt / 2},
					{0, 0, 0, 1, 0, 0, dt, 0, 0},
					{0, 0, 0, 0, 1, 0, 0, dt, 0},
					{0, 0, 0, 0, 0, 1, 0, 0, dt},
					{0, 0, 0, 0, 0, 0, 1, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 1, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 1},
			},
			Matrix {9, 9},
			{Matrix {
					{1, 0, 0},
					{0, 1, 0},
					{0, 0, 1}
			} * uncertainty}
	};

	k.init({
					{0},  // x
					{0},  // y
					{0},  // z
					{0},  // x'
					{0},  // y'
					{0},  // z'
					{0},  // x''
					{0},  // y''
					{0}   // z''
			},
			{
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 0},
					{0, 0, 0, 0, 0, 0, 500, 0, 0},
					{0, 0, 0, 0, 0, 0, 0, 500, 0},
					{0, 0, 0, 0, 0, 0, 0, 0, 500}
			}
	);

	for (size_t i {0}; i < 50; ++i) {
		// Step 1 - Measure
		Matrix Z {
				{1},
				{2},
				{5}
		};

		Matrix H {
				{0, 0, 0, 0, 0, 0, 1, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 1, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 1}
		};

		k.predict();
		k.correct(Z, H);
		std::cout << "Covariance during iteration " << i + 1 << ": " << k.getCovariance() << std::endl;
		std::cout << "Estimated state during iteration " << i + 1 << ": " << k.getState() << std::endl;
	}

	return 0;
}