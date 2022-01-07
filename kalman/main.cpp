//
// Created by mikha on 2.1.22.
//

#include <iostream>
#include <vector>

#include "Matrix.h"


int main() {
	const float dt = 0.02;
	const float g = 9.81;
	const float uncertainty = 0.0392;

	// State extrapolation matrix
	Matrix F {
			{1, 0, 0, dt, 0, 0, dt * dt / 2, 0, 0},
			{0, 1, 0, 0, dt, 0, 0, dt * dt / 2, 0},
			{0, 0, 1, 0, 0, dt, 0, 0, dt * dt / 2},
			{0, 0, 0, 1, 0, 0, dt, 0, 0},
			{0, 0, 0, 0, 1, 0, 0, dt, 0},
			{0, 0, 0, 0, 0, 1, 0, 0, dt},
			{0, 0, 0, 0, 0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 1, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 1},
	};

	// Process noise matrix is zero

	// Observation matrix
	Matrix H {
			{0, 0, 0, 0, 0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 1, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 1}
	};

	// Process uncertainty matrix
	Matrix R {Matrix {
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1}
	} * uncertainty};

	// System state
	Matrix X = {
			{0},  // x
			{0},  // y
			{0},  // z
			{0},  // x'
			{0},  // y'
			{0},  // z'
			{0},  // x''
			{0},  // y''
			{0}   // z''
	};

	// Covariance matrix (estimate uncertainty)
	Matrix P { Matrix {
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 1, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 1, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 1}
	} * uncertainty};

	// Prediction
	X = F * X;
	P = F * P * F.transpose();  // + Q

	for (size_t i {0}; i < 50; ++i) {
		// Step 1 - Measure
		Matrix Z {
				{1},
				{2},
				{5}
		};

		// Step 2 - Update
		// Calculate Kalman gain
		Matrix K {P * H.transpose() * (H * P * H.transpose() + R).invert()};

		// Estimate current state
		X = X + K * (Z - H * X);
		std::cout << "Estimated state during iteration " << i + 1 << ": " << X << std::endl;

		// Update covariance
		Matrix temp {K * H};
		temp = Matrix::identity(temp.getHeight()) - temp;
		P = temp * P * temp.transpose() + K * R * K.transpose();

		// Step 3 - Predict
		X = F * X;
		P = F * P * F.transpose();  // + Q
	}

	return 0;
}