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


class Matrix {
public:
	explicit Matrix(size_t w = 1, size_t h = 1);
	explicit Matrix(const std::vector<scalar>& vector);
	explicit Matrix(const std::vector<std::vector<scalar>>& vector);
	Matrix(const std::initializer_list<std::initializer_list<scalar>>& list);
	static Matrix identity(size_t order);

	void randomize();

	std::vector<scalar>& operator[](size_t i);
	const std::vector<scalar>& operator[](size_t i) const;

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

	explicit operator std::vector<scalar>() const;
	explicit operator std::vector<std::vector<scalar>>() const;

	size_t getWidth() const;
	size_t getHeight() const;

	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
	friend std::istream& operator>>(std::istream& in, Matrix& matrix);

protected:
	std::vector<std::vector<scalar>> _values {};
	size_t _w {};
	size_t _h {};
};

#endif //TRADER_MATRIX_H
