//
// Created by mikha on 25.1.22.
//

#include "AHRS.h"


AHRS::AHRS() :
	_estimator{
		{
			{1, 0},
			{0, 1}
		},
		{
			{0.005, 0, 0, 0},
			{0, 0.005, 0, 0},
			{0, 0, 0.005, 0},
			{0, 0, 0, 0.005},
		},
		4
	} {
	// Nothing to do
}


void AHRS::update(const std::vector<scalar> &acc, const std::vector<scalar> &rot) {

}

void AHRS::update(const std::vector<scalar> &acc, const std::vector<scalar> &rot, const std::vector<scalar> &mag) {

}
