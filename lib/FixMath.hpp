//
// Created by Mikhail on 12.6.23.
//

#ifndef FILTERS_FIXMATH_H
#define FILTERS_FIXMATH_H


#include <type_traits>
#include <cstdint>
#include <cmath>

namespace fix_math {

	constexpr int MIN_INTEGER_BITS = 20;
	constexpr int FRACTIONAL_BITS = 12;

	template<bool>
	struct ChooseType;

	template<>
	struct ChooseType<true> {
		using signedType = int64_t;
		using unsignedType = uint64_t;
	};

	template<>
	struct ChooseType<false> {
		using signedType = int32_t;
		using unsignedType = uint32_t;
	};

	using FIXED_TYPE = ChooseType<(MIN_INTEGER_BITS + FRACTIONAL_BITS > 32)>::signedType;
	constexpr size_t TOTAL_BITS = sizeof(FIXED_TYPE) * 8;
	constexpr size_t INTEGER_BITS = TOTAL_BITS - FRACTIONAL_BITS;
	constexpr FIXED_TYPE ONE = 1 << FRACTIONAL_BITS; // Fixed-point representation of 1
}

class fixed {
public:
	constexpr fixed() = default;
	constexpr explicit fixed(const float& val);
	constexpr explicit fixed(const double& val);
	template<class T>
	constexpr explicit fixed(const T& val);
	constexpr static fixed fromRaw(const fix_math::FIXED_TYPE& val);

	explicit operator float() const;
	explicit operator double() const;
	template<class T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
	explicit operator T();
	[[nodiscard]] fix_math::FIXED_TYPE toRaw() const;

	fixed operator-() const;

	fixed operator+(const fixed& val) const;
	fixed operator-(const fixed& val) const;
	fixed operator%(const fixed& val) const;
	fixed operator*(const fixed& val) const;
	fixed operator/(const fixed& val) const;

	fixed& operator+=(const fixed& val);
	fixed& operator-=(const fixed& val);
	fixed& operator%=(const fixed& val);
	fixed& operator*=(const fixed& val);
	fixed& operator/=(const fixed& val);

	bool operator==(const fixed& val) const;
	bool operator!=(const fixed& val) const;

	bool operator>(const fixed& val) const;
	bool operator>=(const fixed& val) const;
	bool operator<(const fixed& val) const;
	bool operator<=(const fixed& val) const;

private:
	fix_math::FIXED_TYPE _val {0};
};

constexpr fixed::fixed(const float& val) {
	this->_val = static_cast<fix_math::FIXED_TYPE>(val * fix_math::ONE);
}

constexpr fixed::fixed(const double& val) {
	this->_val = static_cast<fix_math::FIXED_TYPE>(val * fix_math::ONE);
}

template<class T>
constexpr fixed::fixed(const T& val) {
	this->_val = static_cast<fix_math::FIXED_TYPE>(static_cast<fix_math::FIXED_TYPE>(val) * fix_math::ONE);
}

template<class T, std::enable_if_t<std::is_integral<T>::value, int>>
fixed::operator T() {
	return static_cast<T>(this->_val / fix_math::ONE);
}

namespace fix_math {
	constexpr fixed FACTORIAL_3 {6};
	constexpr fixed FACTORIAL_5 {120};
	constexpr fixed FACTORIAL_7 {5040};
	constexpr fixed FACTORIAL_9 {362880};

	constexpr fixed F_PI {M_PI};
	constexpr fixed F_PI_2 {M_PI_2};
	constexpr fixed F_PI_4 {M_PI_4};
	constexpr fixed F_2_PI {M_2_PI};
}

	fixed sin(const fixed& x);
	fixed cos(const fixed& x);
	fixed tan(const fixed& x);

	fixed asin(const fixed& x);
	fixed acos(const fixed& x);
	fixed atan(const fixed& x);
	fixed atan2(const fixed& y, const fixed& x);

#endif //FILTERS_FIXMATH_H
