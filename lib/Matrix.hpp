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


class Matrix {
public:
	using size_type = unsigned int;
	using scalar = float;
	using allocator_type = tl::allocator<scalar>;

	Matrix() = default;
	explicit Matrix(size_type w, size_type h);
	Matrix(const std::initializer_list<std::initializer_list<scalar>>& list);

	Matrix(const Matrix& matrix);
	Matrix& operator=(const Matrix& matrix);

	~Matrix();

	static Matrix identity(size_type order);

	scalar* operator[](size_type i);
	const scalar* operator[](size_type i) const;

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

	size_type getWidth() const;
	size_type getHeight() const;

	#ifdef MATRIX_IO
	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
	#endif

protected:
	size_type _h {0};
	size_type _w {0};
	scalar* _values {};
};

#endif //TRADER_MATRIX_H
