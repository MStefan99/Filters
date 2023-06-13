//
// Created by Mikhail on 12.6.23.
//

#include "FixMath.hpp"


fix_math::fixed::operator float() const {
	return static_cast<float>(this->_val) / ONE;
}

fix_math::fixed::operator double() const {
	return static_cast<double>(this->_val) / ONE;
}

constexpr fix_math::fixed fix_math::fixed::fromRaw(const FIXED_TYPE& val) {
	fixed f;
	f._val = val;
	return f;
}

fix_math::FIXED_TYPE fix_math::fixed::toRaw() const {
	return this->_val;
}

fix_math::fixed fix_math::fixed::operator-() const {
	return fixed::fromRaw(-this->_val);
}

fix_math::fixed fix_math::fixed::operator+(const fixed& val) const {
	return fixed::fromRaw(this->_val + val._val);
}

fix_math::fixed fix_math::fixed::operator-(const fixed& val) const {
	return fixed::fromRaw(this->_val - val._val);
}

fix_math::fixed fix_math::fixed::operator%(const fix_math::fixed& val) const {
	return fixed::fromRaw(this->_val % val._val);
}

fix_math::fixed fix_math::fixed::operator*(const fixed& val) const {
	// The operation will increase the fixed-point precision, so we need to shift right to get back to the correct format.
	auto result = static_cast<FIXED_TYPE>((static_cast<int64_t>(_val) * val._val) >> FRACTIONAL_BITS);
	return fixed::fromRaw(result);
}

fix_math::fixed fix_math::fixed::operator/(const fixed& val) const {
	// The operation will decrease the fixed-point precision, so we need to shift left before the division.
	auto result = static_cast<FIXED_TYPE>((static_cast<int64_t>(_val) << FRACTIONAL_BITS) / val._val);
	return fixed::fromRaw(result);
}

fix_math::fixed& fix_math::fixed::operator+=(const fixed& val) {
	this->_val += val._val;
	return *this;
}

fix_math::fixed& fix_math::fixed::operator-=(const fixed& val) {
	this->_val -= val._val;
	return *this;
}

fix_math::fixed& fix_math::fixed::operator%=(const fix_math::fixed& val) {
	this->_val %= val._val;
	return *this;
}

fix_math::fixed& fix_math::fixed::operator*=(const fixed& val) {
	// The operation will increase the fixed-point precision, so we need to shift right to get back to the correct format.
	this->_val = static_cast<FIXED_TYPE>((static_cast<int64_t>(_val) * val._val) >> FRACTIONAL_BITS);
	return *this;
}

fix_math::fixed& fix_math::fixed::operator/=(const fixed& val) {
	// The operation will decrease the fixed-point precision, so we need to shift left before the division.
	this->_val = static_cast<FIXED_TYPE>((static_cast<int64_t>(_val) << FRACTIONAL_BITS) / val._val);
	return *this;
}

bool fix_math::fixed::operator==(const fix_math::fixed& val) const {
	return this->_val == val._val;
}

bool fix_math::fixed::operator!=(const fix_math::fixed& val) const {
	return this->_val != val._val;
}

bool fix_math::fixed::operator>(const fix_math::fixed& val) const {
	return this->_val > val._val;
}

bool fix_math::fixed::operator>=(const fix_math::fixed& val) const {
	return this->_val >= val._val;
}

bool fix_math::fixed::operator<(const fix_math::fixed& val) const {
	return this->_val < val._val;
}

bool fix_math::fixed::operator<=(const fix_math::fixed& val) const {
	return this->_val <= val._val;
}

fix_math::fixed fix_math::sin(const fix_math::fixed& val) {
	// Wrap x to the range -pi to pi
	fix_math::fixed x {val};
	x %= fixed(2) * fix_math::fixed(F_PI);
	if (x > fix_math::fixed(F_PI)) x -= fixed(2) * fix_math::fixed(F_PI);
	if (x < fix_math::fixed(-F_PI)) x += fixed(2) * fix_math::fixed(F_PI);

	fix_math::fixed x2 = x * x; // x^2
	fix_math::fixed x3 = x * x2; // x^3
	fix_math::fixed x5 = x3 * x2; // x^5
	fix_math::fixed x7 = x5 * x2; // x^7

	// Approximate sin(x) using Taylor series: x - x^3/3! + x^5/5! - x^7/7! + x^9/9!
	if (INTEGER_BITS < 18) {
		return x - ((x3 / fix_math::FACTORIAL_3) - (x5 / fix_math::FACTORIAL_5) + (x7 / fix_math::FACTORIAL_7));
	} else {
		fix_math::fixed x9 = x7 * x2; // x^9

		return x - ((x3 / fix_math::FACTORIAL_3) - (x5 / fix_math::FACTORIAL_5) + (x7 / fix_math::FACTORIAL_7) -
				(x9 / fix_math::FACTORIAL_9));
	}
}

fix_math::fixed fix_math::cos(const fix_math::fixed& x) {
	return fix_math::sin(x + F_PI_2);
}

fix_math::fixed fix_math::tan(const fix_math::fixed& val) {
	fix_math::fixed x {val};
	x %= fixed(2) * fix_math::fixed(F_PI_2);
	if (x > fix_math::fixed(F_PI_2)) x -= fix_math::fixed(F_PI);
	if (x < fix_math::fixed(-F_PI_2)) x += fix_math::fixed(F_PI);

	fix_math::fixed x2 = x * x; // x^2
	fix_math::fixed x3 = x * x2; // x^3
	fix_math::fixed x5 = x3 * x2; // x^5
	fix_math::fixed x7 = x5 * x2; // x^7
	fix_math::fixed x9 = x7 * x2; // x^9

	// Approximate tan(x) using Taylor series: x + x^3/3 + 2x^5/15 + 17x^7/315 + 62x^9/2835
	return x + (x3 / fixed(3)) + (fixed(2) * x5 / fixed(15)) +
			(fixed(17) * x7 / fixed(315)) + (fixed(62)) * x9 / fixed(2835);
}

fix_math::fixed fix_math::asin(const fix_math::fixed& val) {
	fix_math::fixed x = val % fixed(1);

	fix_math::fixed x2 = x * x; // x^2
	fix_math::fixed x3 = x * x2; // x^3
	fix_math::fixed x5 = x3 * x2; // x^5
	fix_math::fixed x7 = x5 * x2; // x^7
	fix_math::fixed x9 = x7 * x2; // x^9

	// Approximate arcsin(x) using Taylor series: x + x^3/6 + 3x^5/40 + 5x^7/112 + 35x^9/1152
	return x + (x3 / fixed(6)) + (fixed(3) * x5 / fixed(40)) +
			(fixed(5) * x7 / fixed(112) + (fixed(35)) * x9 / fixed(1152));
}

fix_math::fixed fix_math::acos(const fix_math::fixed& x) {
	return F_PI_2 - asin(x);
}

static fix_math::fixed atan_1(const fix_math::fixed& val) {
	fix_math::fixed x = val % fix_math::fixed(1);

	fix_math::fixed x2 = x * x; // x^2
	fix_math::fixed x3 = x * x2; // x^3
	fix_math::fixed x5 = x3 * x2; // x^5
	fix_math::fixed x7 = x5 * x2; // x^7
	fix_math::fixed x9 = x7 * x2; // x^9

	// Approximate arctan(x) using Taylor series: x - x^3/3 + x^5/5 - x^7/7 + x^9/9
	return x - (x3 / fix_math::fixed(3) - x5 / fix_math::fixed(5) + x7 / fix_math::fixed(7) - x9 / fix_math::fixed(9));
}

fix_math::fixed fix_math::atan(const fix_math::fixed& x) {
	if (x > fixed(1)) {
		// if x > 1, use atan(x) = pi/2 - atan(1/x)
		return F_PI_2 - atan_1(fixed(1) / x);
	} else if (x < fixed(-1)) {
		// if x < -1, use atan(x) = -pi/2 - atan(1/x)
		return -F_PI_2 - atan_1(fixed(1) / x);
	} else {
		// if -1 <= x <= 1, just use the atan approximation
		return atan_1(x);
	}
}

fix_math::fixed fix_math::atan2(const fix_math::fixed& y, const fix_math::fixed& x) {
	if (x > fixed(0)) {
		return atan(y / x);
	} else if (x < fixed(0)) {
		if (y >= fixed(0)) return atan(y / x) + F_PI;
		else return atan(y / x) - F_PI;
	} else {
		if (y > fixed(0)) return F_PI_2;
		else if (y < fixed(0)) return -F_PI_2;
		else return fixed(0); // undefined, but let's return 0
	}
}
