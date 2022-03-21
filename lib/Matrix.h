//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_MATRIX_H
#define TRADER_MATRIX_H

#include <cstdint>
#include <initializer_list>

#include "allocator.hpp"


#ifdef MATRIX_IO

	#include <iostream>

#endif


typedef float scalar;
typedef uint16_t size;


class Matrix {
public:
	using allocator_type = tl::allocator<scalar>;

	Matrix() = default;
	explicit Matrix(size w, size h);
	Matrix(const std::initializer_list<std::initializer_list<scalar>>& list);

	Matrix(const Matrix& matrix);
	Matrix& operator=(const Matrix& matrix);

	~Matrix();

	static Matrix identity(size order);

	scalar* operator[](size i);
	const scalar* operator[](size i) const;

	Matrix transpose() const;
	Matrix invert() const;

	Matrix operator*(scalar multiplier) const;
	Matrix& operator*=(scalar multiplier);

	Matrix operator/(scalar divisor) const;
	Matrix& operator/=(scalar divisor);

	Matrix operator+(const Matrix& matrix) const;
	Matrix operator-(const Matrix& matrix) const;
	Matrix operator*(const Matrix& matrix) const;

	Matrix multiplyComponents(const Matrix& matrix) const;
	Matrix concat(const Matrix& matrix) const;

	size getWidth() const;
	size getHeight() const;

	#ifdef MATRIX_IO
	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
	#endif

protected:
	size _h {0};
	size _w {0};
	scalar* _values {};
};

#endif //TRADER_MATRIX_H
