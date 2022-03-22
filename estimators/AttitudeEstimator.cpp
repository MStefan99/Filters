//
// Created by MStefan99 on 21.3.22.
//

#include "AttitudeEstimator.h"


/*
 * Roll - phi
 * pitch - theta
 */


#define processNoise 0.005


AttitudeEstimator::AttitudeEstimator(scalar accUncertainty):
		_k({
						{processNoise, 0},
						{0, processNoise}
				},
				{
						{accUncertainty, 0, 0},
						{0, accUncertainty, 0},
						{0, 0, accUncertainty}
				}) {
// Nothing to do
}


//void AttitudeEstimator::init(float roll, float pitch) {
//	_k.init(F, {{roll}, {pitch}},
//			{180, 0, 0},
//			{0, 180, 0},
//			{0, 0, 180});
//}


//void AttitudeEstimator::update(scalar dt) {
//	_k.extrapolateState(F * dt);
//}


//Matrix AttitudeEstimator::getAttitude() const {
//	return _k.getState();
//}


