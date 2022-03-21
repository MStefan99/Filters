//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_MATRIX_H
#define TRADER_MATRIX_H

#include <vector>
#include <stdexcept>
#include <random>
#include <iostream>

#include "input_validation.h"

typedef double scalar;
typedef size_t size;
#define TL std


class Matrix {
public:
	Matrix() = default;
	explicit Matrix(size w, size h);
	explicit Matrix(const TL::vector<scalar>& vector);
	explicit Matrix(const TL::vector<TL::vector<scalar>>& vector);
	Matrix(const TL::initializer_list<TL::initializer_list<scalar>>& list);
	static Matrix identity(size order);

	void randomize();

	TL::vector<scalar>& operator[](size i);
	const TL::vector<scalar>& operator[](size i) const;

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

	explicit operator TL::vector<scalar>() const;
	explicit operator TL::vector<TL::vector<scalar>>() const;

	size getWidth() const;
	size getHeight() const;

	friend TL::ostream& operator<<(TL::ostream& out, const Matrix& matrix);
	friend TL::istream& operator>>(TL::istream& in, Matrix& matrix);

protected:
	TL::vector<TL::vector<scalar>> _values {};
	size _w {0};
	size _h {0};
};

#endif //TRADER_MATRIX_H
