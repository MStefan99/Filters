//
// Created by mikha on 2.1.22.
//

#include <iostream>

#include "Kalman.hpp"


constexpr auto x = Matrix<float,
		unsigned, 3, 1> {{0},
                     {0},
                     {0}};
constexpr auto P = Matrix<float,
		unsigned, 3, 3> {{0, 0, 0},
                     {0, 0, 0},
                     {0, 0, 0}};
constexpr auto Q = Matrix<float,
		unsigned, 3, 3> {{0.1, 0,   0},
                     {0,   0.1, 0},
                     {0,   0,   0.1}};
constexpr auto R = Matrix<float,
		unsigned, 1, 1> {{1}};

constexpr float dt {0.001f};


int main() {
	auto kalman = Kalman<float, unsigned, 3, 1, 1> {x, P, Q, R};

	auto F = Matrix<float, unsigned, 3, 3> {{1, dt, dt * dt / 2},
	                                        {0, 1,  dt},
	                                        {0, 0,  1}};
	auto G = Matrix<float, unsigned, 3, 1> {{0},
	                                        {0},
	                                        {1}};
	auto u = Matrix<float, unsigned, 1, 1> {{0}};
	auto H = Matrix<float, unsigned, 1, 3> {{1, 0, 0}};
	auto z = Matrix<float, unsigned, 1, 1> {{0}};

	kalman.predict(F, G, u);
	kalman.correct(H, z);

	std::cout << "x:" << std::endl << kalman.x() << std::endl << "P:" << std::endl << kalman.P() << std::endl;

	return 0;
}
