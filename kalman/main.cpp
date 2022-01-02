//
// Created by mikha on 2.1.22.
//

#include <iostream>
#include <vector>

#include "Matrix.h"


int main() {
	std::vector<float> x {-393.66, -375.93, -351.04, -328.96, -299.35, -273.36, -245.89, -222.58, -198.03, -174.17,
			-146.32, -123.72, -103.47, -78.23, -52.63, -23.34, 25.96, 49.72, 76.94, 95.38, 119.83, 144.01, 161.84, 180.56,
			201.42, 222.62, 239.4, 252.51, 266.26, 271.75, 277.4, 294.12, 301.23, 291.8, 299.89};
	std::vector<float> y {300.4, 301.78, 295.1, 305.19, 301.06, 302.05, 300, 303.57, 296.33, 297.65, 297.41, 299.61,
			299.6, 302.39, 295.04, 300.09, 294.72, 298.61, 294.64, 284.88, 272.82, 264.93, 251.46, 241.27, 222.98, 203.73,
			184.1, 166.12, 138.71, 119.71, 100.41, 79.76, 50.62, 32.99, 2.14};


	// State extrapolation matrix
	Matrix F {
			{1, 1, 0.5, 0, 0, 0},
			{0, 1, 1, 0, 0, 0},
			{0, 0, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0.5},
			{0, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 1}
	};

	// Process noise matrix
	Matrix Q {Matrix {
			{0.25, 0.5, 0.5, 0, 0, 0},
			{0.5, 1, 1, 0, 0, 0},
			{0.5, 1, 1, 0, 0, 0},
			{0, 0, 0, 0.25, 0.5, 0.5},
			{0, 0, 0, 0.5, 1, 1},
			{0, 0, 0, 0.5, 1, 1}
	} * 0.15 * 0.15};

	// Observation matrix
	Matrix H {
			{1, 0, 0, 0, 0, 0},
			{0, 0, 0, 1, 0, 0}
	};

	// Process uncertainty matrix
	Matrix R {
			{9, 0},
			{0, 9}
	};

	// System state
	Matrix X = {
			{0},
			{0},
			{0},
			{0},
			{0},
			{0}
	};

	// Covariance matrix (estimate uncertainty)
	Matrix P {
			{500, 0, 0, 0, 0, 0},
			{0, 500, 0, 0, 0, 0},
			{0, 0, 500, 0, 0, 0},
			{0, 0, 0, 500, 0, 0},
			{0, 0, 0, 0, 500, 0},
			{0, 0, 0, 0, 0, 500}
	};

	// Prediction
	X = F * X;
	P = F * P * F.transpose() + Q;

	for (size_t i {0}; i < x.size(); ++i) {
		// Step 1 - Measure
		Matrix Z {
				{x[i]},
				{y[i]}
		};

		// Step 2 - Update
		// Calculate Kalman gain
		Matrix K {P * H.transpose() * (H * P * H.transpose() + R).invert()};

		// Estimate current state
		X = X + K * (Z - H * X);
		std::cout << "Estimated state during iteration " << i + 1 << ": " << X << std::endl;

		// Update covariance
		Matrix temp {K * H};
		P = (Matrix::identity(temp.getHeight()) - temp) * P
				* (Matrix::identity(temp.getHeight()) - temp).transpose()
				+ K * R * K.transpose();

		// Step 3 - Predict
		X = F * X;
		P = F * P * F.transpose() + Q;
	}

	return 0;
}