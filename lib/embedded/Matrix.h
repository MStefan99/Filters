//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_MATRIX_H
#define TRADER_MATRIX_H

#include <vector>
#include <cstdint>


typedef float scalar;
typedef uint16_t size;
#define TL std


class Matrix {
public:
	Matrix() = default;
	explicit Matrix(size w, size);
	explicit Matrix(const TL::vector<scalar>& vector);
	explicit Matrix(const TL::vector<TL::vector<scalar>>& vector);
	Matrix(const TL::initializer_list<TL::initializer_list<scalar>>& list);
	static Matrix identity(size order);

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

protected:
	TL::vector<TL::vector<scalar>> _values {};
	size _w {0};
	size _h {0};
};

#endif //TRADER_MATRIX_H
