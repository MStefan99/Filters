//
// Created by Mikhail on 12.6.23.
//

#include "FixMath.hpp"


fixed::operator float() const {
	return static_cast<float>(this->_val) / fix_math::ONE;
}

fixed::operator double() const {
	return static_cast<double>(this->_val) / fix_math::ONE;
}

constexpr fixed fixed::fromRaw(const fix_math::FIXED_TYPE& val) {
	fixed f;
	f._val = val;
	return f;
}

fix_math::FIXED_TYPE fixed::toRaw() const {
	return this->_val;
}

fixed fixed::operator-() const {
	return fixed::fromRaw(-this->_val);
}

fixed fixed::operator+(const fixed& val) const {
	return fixed::fromRaw(this->_val + val._val);
}

fixed fixed::operator-(const fixed& val) const {
	return fixed::fromRaw(this->_val - val._val);
}

fixed fixed::operator%(const fixed& val) const {
	return fixed::fromRaw(this->_val % val._val);
}

fixed fixed::operator*(const fixed& val) const {
	// The operation will increase the fixed-point precision, so we need to shift right to get back to the correct format.
	auto result = static_cast<fix_math::FIXED_TYPE>((static_cast<int64_t>(_val) * val._val) >> fix_math::FRACTIONAL_BITS);
	return fixed::fromRaw(result);
}

fixed fixed::operator/(const fixed& val) const {
	// The operation will decrease the fixed-point precision, so we need to shift left before the division.
	auto result = static_cast<fix_math::FIXED_TYPE>((static_cast<int64_t>(_val) << fix_math::FRACTIONAL_BITS) / val._val);
	return fixed::fromRaw(result);
}

fixed& fixed::operator+=(const fixed& val) {
	this->_val += val._val;
	return *this;
}

fixed& fixed::operator-=(const fixed& val) {
	this->_val -= val._val;
	return *this;
}

fixed& fixed::operator%=(const fixed& val) {
	this->_val %= val._val;
	return *this;
}

fixed& fixed::operator*=(const fixed& val) {
	// The operation will increase the fixed-point precision, so we need to shift right to get back to the correct format.
	this->_val = static_cast<fix_math::FIXED_TYPE>((static_cast<int64_t>(_val) * val._val) >> fix_math::FRACTIONAL_BITS);
	return *this;
}

fixed& fixed::operator/=(const fixed& val) {
	// The operation will decrease the fixed-point precision, so we need to shift left before the division.
	this->_val = static_cast<fix_math::FIXED_TYPE>((static_cast<int64_t>(_val) << fix_math::FRACTIONAL_BITS) / val._val);
	return *this;
}

bool fixed::operator==(const fixed& val) const {
	return this->_val == val._val;
}

bool fixed::operator!=(const fixed& val) const {
	return this->_val != val._val;
}

bool fixed::operator>(const fixed& val) const {
	return this->_val > val._val;
}

bool fixed::operator>=(const fixed& val) const {
	return this->_val >= val._val;
}

bool fixed::operator<(const fixed& val) const {
	return this->_val < val._val;
}

bool fixed::operator<=(const fixed& val) const {
	return this->_val <= val._val;
}

fixed sin(const fixed& val) {
	// Wrap x to the range -pi to pi
	fixed x {val};
	x %= fixed(2) * fixed(fix_math::F_PI);
	if (x > fixed(fix_math::F_PI)) x -= fixed(2) * fixed(fix_math::F_PI);
	if (x < fixed(-fix_math::F_PI)) x += fixed(2) * fixed(fix_math::F_PI);

	fixed x2 = x * x; // x^2
	fixed x3 = x * x2; // x^3
	fixed x5 = x3 * x2; // x^5
	fixed x7 = x5 * x2; // x^7

	// Approximate sin(x) using Taylor series: x - x^3/3! + x^5/5! - x^7/7! + x^9/9!
	if (fix_math::INTEGER_BITS < 18) {
		return x - ((x3 / fix_math::FACTORIAL_3) - (x5 / fix_math::FACTORIAL_5) + (x7 / fix_math::FACTORIAL_7));
	} else {
		fixed x9 = x7 * x2; // x^9

		return x - ((x3 / fix_math::FACTORIAL_3) - (x5 / fix_math::FACTORIAL_5) + (x7 / fix_math::FACTORIAL_7) -
				(x9 / fix_math::FACTORIAL_9));
	}
}

fixed cos(const fixed& x) {
	return sin(x + fix_math::F_PI_2);
}

fixed tan(const fixed& val) {
	fixed x {val};
	x %= fixed(2) * fixed(fix_math::F_PI_2);
	if (x > fixed(fix_math::F_PI_2)) x -= fixed(fix_math::F_PI);
	if (x < fixed(-fix_math::F_PI_2)) x += fixed(fix_math::F_PI);

	fixed x2 = x * x; // x^2
	fixed x3 = x * x2; // x^3
	fixed x5 = x3 * x2; // x^5
	fixed x7 = x5 * x2; // x^7
	fixed x9 = x7 * x2; // x^9

	// Approximate tan(x) using Taylor series: x + x^3/3 + 2x^5/15 + 17x^7/315 + 62x^9/2835
	return x + (x3 / fixed(3)) + (fixed(2) * x5 / fixed(15)) +
			(fixed(17) * x7 / fixed(315)) + (fixed(62)) * x9 / fixed(2835);
}

fixed asin(const fixed& val) {
	fixed x = val % fixed(1);

	fixed x2 = x * x; // x^2
	fixed x3 = x * x2; // x^3
	fixed x5 = x3 * x2; // x^5
	fixed x7 = x5 * x2; // x^7
	fixed x9 = x7 * x2; // x^9

	// Approximate arcsin(x) using Taylor series: x + x^3/6 + 3x^5/40 + 5x^7/112 + 35x^9/1152
	return x + (x3 / fixed(6)) + (fixed(3) * x5 / fixed(40)) +
			(fixed(5) * x7 / fixed(112) + (fixed(35)) * x9 / fixed(1152));
}

fixed acos(const fixed& x) {
	return fix_math::F_PI_2 - asin(x);
}

static fixed atan_1(const fixed& val) {
	fixed x = val % fixed(1);

	fixed x2 = x * x; // x^2
	fixed x3 = x * x2; // x^3
	fixed x5 = x3 * x2; // x^5
	fixed x7 = x5 * x2; // x^7
	fixed x9 = x7 * x2; // x^9

	// Approximate arctan(x) using Taylor series: x - x^3/3 + x^5/5 - x^7/7 + x^9/9
	return x - (x3 / fixed(3) - x5 / fixed(5) + x7 / fixed(7) - x9 / fixed(9));
}

fixed atan(const fixed& x) {
	if (x > fixed(1)) {
		// if x > 1, use atan(x) = pi/2 - atan(1/x)
		return fix_math::F_PI_2 - atan_1(fixed(1) / x);
	} else if (x < fixed(-1)) {
		// if x < -1, use atan(x) = -pi/2 - atan(1/x)
		return -fix_math::F_PI_2 - atan_1(fixed(1) / x);
	} else {
		// if -1 <= x <= 1, just use the atan approximation
		return atan_1(x);
	}
}

fixed atan2(const fixed& y, const fixed& x) {
	if (x > fixed(0)) {
		return atan(y / x);
	} else if (x < fixed(0)) {
		if (y >= fixed(0)) return atan(y / x) + fix_math::F_PI;
		else return atan(y / x) - fix_math::F_PI;
	} else {
		if (y > fixed(0)) return fix_math::F_PI_2;
		else if (y < fixed(0)) return -fix_math::F_PI_2;
		else return fixed(0); // undefined, but let's return 0
	}
}
