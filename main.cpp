//
// Created by mikha on 2.1.22.
//

#include <iostream>
#include <vector>

#define MATRIX_IO

#include "Kalman.h"
#include "Matrix.h"


int main() {
	std::vector<float> x {-393.66, -375.93, -351.04, -328.96, -299.35, -273.36, -245.89, -222.58, -198.03, -174.17,
			-146.32, -123.72, -103.47, -78.23, -52.63, -23.34, 25.96, 49.72, 76.94, 95.38, 119.83, 144.01, 161.84, 180.56,
			201.42, 222.62, 239.4, 252.51, 266.26, 271.75, 277.4, 294.12, 301.23, 291.8, 299.89};
	std::vector<float> y {300.4, 301.78, 295.1, 305.19, 301.06, 302.05, 300, 303.57, 296.33, 297.65, 297.41, 299.61,
			299.6, 302.39, 295.04, 300.09, 294.72, 298.61, 294.64, 284.88, 272.82, 264.93, 251.46, 241.27, 222.98, 203.73,
			184.1, 166.12, 138.71, 119.71, 100.41, 79.76, 50.62, 32.99, 2.14};

	Kalman<6, 2> k {
			// Process noise matrix Q
			Matrix<6, 6> {
					{0.25, 0.5, 0.5, 0, 0, 0},
					{0.5, 1, 1, 0, 0, 0},
					{0.5, 1, 1, 0, 0, 0},
					{0, 0, 0, 0.25, 0.5, 0.5},
					{0, 0, 0, 0.5, 1, 1},
					{0, 0, 0, 0.5, 1, 1}
			} * 0.15 * 0.15,
			// Process uncertainty matrix R
			Matrix<2, 2> {
					{9, 0},
					{0, 9}
			}
	};

	// State extrapolation matrix F
	Matrix<6, 6> F {
			{1, 1, 0.5, 0, 0, 0},
			{0, 1, 1, 0, 0, 0},
			{0, 0, 1, 0, 0, 0},
			{0, 0, 0, 1, 1, 0.5},
			{0, 0, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 1}
	};

	k.init(F,
			// Covariance matrix P
			Matrix<6, 6> {
					{500, 0, 0, 0, 0, 0},
					{0, 500, 0, 0, 0, 0},
					{0, 0, 500, 0, 0, 0},
					{0, 0, 0, 500, 0, 0},
					{0, 0, 0, 0, 500, 0},
					{0, 0, 0, 0, 0, 500}
			},
			// System state X
			Vector<6> {
					{0},
					{0},
					{0},
					{0},
					{0},
					{0}
			}
	);

	// Observation matrix
	Matrix<2, 6> H {
			{1, 0, 0, 0, 0, 0},
			{0, 0, 0, 1, 0, 0}
	};


	k.predict(F);

	for (size_t i {0}; i < x.size(); ++i) {
		Vector<2> Z {
				{x[i]},
				{y[i]}
		};

		k.correct(Z, H);
		std::cout << "Estimated state during iteration " << i + 1 << ": " << k.getCovariance() << std::endl;

		k.predict(F);
	}

	return 0;
}