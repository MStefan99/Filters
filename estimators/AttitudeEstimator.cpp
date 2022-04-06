//
// Created by MStefan99 on 21.3.22.
//

#include "AttitudeEstimator.h"


#define processNoise 0.005f
#define g 9.81f
#define pi 3.1415f

#define phi x[0][0]
#define theta x[1][0]

#define p u[0][0]
#define q u[1][0]
#define r u[2][0]


Matrix extrapolateState(const Matrix& x, const Matrix& u, float dt) {
	float sin_phi = sinf(phi);
	float cos_phi = cosf(phi);
	float tan_theta = tanf(theta);

	return {
			{phi + (p + q * sin_phi * tan_theta + r * cos_phi * tan_theta) * dt},
			{theta + (q * cos_phi + r * sin_phi) * dt}
	};
}


Matrix getOutput(const Matrix& x) {
	float sin_phi = sinf(phi);
	float sin_theta = sinf(theta);
	float cos_phi = cosf(phi);
	float cos_theta = cosf(theta);

	return {
			{-g * sin_theta},
			{g * cos_theta * sin_phi},
			{g * cos_theta * cos_phi}
	};
}


Matrix getF(const Matrix& x, const Matrix& u, float dt) {
	float sin_phi = sinf(phi);
	float cos_phi = cosf(phi);

	float tan_theta = tanf(theta);
	float tan_s = tan_theta * tan_theta + 1;

	return {
			{dt * (q * cos_phi * tan_theta - r * sin_phi * tan_theta) + 1,
					dt * (r * cos_phi * tan_s + q * sin_phi * tan_s)},
			{dt * (r * cos_phi - q * sin_phi), 1}
	};
}


Matrix getH(const Matrix& x) {
	float sin_phi = sinf(phi);
	float sin_theta = sinf(theta);
	float cos_phi = cosf(phi);
	float cos_theta = cosf(theta);

	return {
			{0, -g * cos_theta},
			{g * cos_phi * cos_theta, -g * sin_phi * sin_theta},
			{-g * cos_theta * sin_phi, -g * cos_phi * sin_theta}
	};
}


AttitudeEstimator::AttitudeEstimator(float accUncertainty):
		_k {
				{
						{processNoise, 0},
						{0, processNoise}
				},
				{
						{accUncertainty, 0, 0},
						{0, accUncertainty, 0},
						{0, 0, accUncertainty}
				},
				&extrapolateState,
				&getOutput,
				&getF,
				&getH} {
// Nothing to do
}


void AttitudeEstimator::init(float roll, float pitch) {
	_k.init(
			{
					{pi, 0},
					{0, pi}
			},
			{
					{roll},
					{pitch}
			},
			{
					{0},
					{0},
					{g}
			}
	);
}


void AttitudeEstimator::update(float dt) {
	_k.extrapolateState(_u, dt);
}


void AttitudeEstimator::measure(const Matrix& rot, const Matrix& acc, float dt) {
	_u = rot;
	_k.extrapolateState(rot, dt);
	_k.updateState(acc);
}


float AttitudeEstimator::getRoll() const {
	return _k.getState()[0][0];
}


float AttitudeEstimator::getPitch() const {
	return _k.getState()[1][0];
}


Matrix AttitudeEstimator::getUncertainty() const {
	return _k.getCovariance();
}
