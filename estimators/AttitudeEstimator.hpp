//
// Created by MStefan99 on 21.3.22.
//

#ifndef FILTERS_ATTITUDEESTIMATOR_HPP
#define FILTERS_ATTITUDEESTIMATOR_HPP

#include <cmath>
#include "FixMath.hpp"
#include "EKF.hpp"


template<class scalar = fixed, class size_type = unsigned int>
class AttitudeEstimator {
public:
	static constexpr scalar processNoise = 0.005f;
	static constexpr scalar g = 9.81f;
	static constexpr scalar pi = 3.1415f;

	explicit AttitudeEstimator(scalar accUncertainty);
	void init(scalar roll, scalar pitch);

	void update(scalar dt);
	void measure(
			const Matrix<scalar, size_type>& rot,
			const Matrix<scalar, size_type>& acc,
			float dt);

	[[nodiscard]] scalar getRoll() const;
	[[nodiscard]] scalar getPitch() const;

	Matrix<scalar, size_type> getUncertainty() const;

protected:
	Matrix<scalar, size_type> _u {};

	EKF<scalar, size_type> _k;
};

#define phi x[0][0]
#define theta x[1][0]

#define p _u[0][0]
#define q _u[1][0]
#define r _u[2][0]

template<class scalar, class size_type>
AttitudeEstimator<scalar, size_type>::AttitudeEstimator(scalar accUncertainty):
		_k {
				{
						{processNoise,   0},
						{0, processNoise}
				},
				{
						{accUncertainty, 0, 0},
						{0, accUncertainty, 0},
						{0, 0, accUncertainty}
				}
		} {
// Nothing to do
}


template<class scalar, class size_type>
void AttitudeEstimator<scalar, size_type>::init(scalar roll, scalar pitch) {
	_k.init(
			{
					{pi, 0},
					{0,  pi}
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


template<class scalar, class size_type>
void AttitudeEstimator<scalar, size_type>::update(scalar dt) {
	const Matrix<scalar, size_type>& x = _k.getState();
	scalar sin_phi = sin(phi);
	scalar cos_phi = cos(phi);
	scalar tan_theta = tan(theta);
	scalar tan_s = tan_theta * tan_theta + 1;

	Matrix<scalar, size_type> xe {
			{phi + (p + q * sin_phi * tan_theta + r * cos_phi * tan_theta) * dt},
			{theta + (q * cos_phi + r * sin_phi) * dt}
	};

	Matrix<scalar, size_type> F {
			{dt * (q * cos_phi * tan_theta - r * sin_phi * tan_theta) + 1,
																				 dt * (r * cos_phi * tan_s + q * sin_phi * tan_s)},
			{dt * (r * cos_phi - q * sin_phi), 1},
	};

	_k.extrapolateState(xe, F);
}


template<class scalar, class size_type>
void AttitudeEstimator<scalar, size_type>::measure(const Matrix<scalar, size_type>& rot,
		const Matrix<scalar, size_type>& acc, float dt) {
	const Matrix<scalar, size_type>& x = _k.getState();
	_u = rot;

	{
		scalar sin_phi = sin(phi);
		scalar cos_phi = cos(phi);
		scalar tan_theta = tan(theta);
		scalar tan_s = tan_theta * tan_theta + 1;

		Matrix<scalar, size_type> xe {
				{phi + (p + q * sin_phi * tan_theta + r * cos_phi * tan_theta) * dt},
				{theta + (q * cos_phi + r * sin_phi) * dt}
		};

		Matrix<scalar, size_type> F {
				{dt * (q * cos_phi * tan_theta - r * sin_phi * tan_theta) + 1,
																					 dt * (r * cos_phi * tan_s + q * sin_phi * tan_s)},
				{dt * (r * cos_phi - q * sin_phi), 1},
		};

		_k.extrapolateState(xe, F);
	}

	{
		scalar sin_phi = sinf(phi);
		scalar cos_phi = cosf(phi);
		scalar sin_theta = sinf(theta);
		scalar cos_theta = cosf(theta);
		scalar tan_theta = tanf(theta);
		scalar tan_s = tan_theta * tan_theta + 1;

		Matrix<scalar, size_type> H {
				{0,                        -g * cos_theta},
				{g * cos_phi * cos_theta,  -g * sin_phi * sin_theta},
				{-g * cos_theta * sin_phi, -g * cos_phi * sin_theta}
		};

		Matrix<scalar, size_type> out {
				{-g * sin_theta},
				{g * cos_theta * sin_phi},
				{g * cos_theta * cos_phi}
		};

		_k.updateState(H, acc, out);
	}
}


template<class scalar, class size_type>
scalar AttitudeEstimator<scalar, size_type>::getRoll() const {
	return _k.getState()[0][0];
}


template<class scalar, class size_type>
scalar AttitudeEstimator<scalar, size_type>::getPitch() const {
	return _k.getState()[1][0];
}


template<class scalar, class size_type>
Matrix<scalar, size_type> AttitudeEstimator<scalar, size_type>::getUncertainty() const {
	return _k.getCovariance();
}

#undef phi
#undef theta

#undef p
#undef q
#undef r

#endif //FILTERS_ATTITUDEESTIMATOR_HPP
