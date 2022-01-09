//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_MATRIX_H
#define TRADER_MATRIX_H

#include <vector>
#include <cstdint>


typedef float scalar;
typedef uint16_t size;


class Matrix {
public:
	explicit Matrix(size w = 1, size h = 1);
	explicit Matrix(const std::vector<scalar>& vector);
	explicit Matrix(const std::vector<std::vector<scalar>>& vector);
	Matrix(const std::initializer_list<std::initializer_list<scalar>>& list);
	static Matrix identity(size order);

	std::vector<scalar>& operator[](size i);
	const std::vector<scalar>& operator[](size i) const;

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

	size getWidth() const;
	size getHeight() const;

protected:
	std::vector<std::vector<scalar>> _values {};
	size _w {};
	size _h {};
};

#endif //TRADER_MATRIX_H
