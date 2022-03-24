//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_MATRIX_H
#define TRADER_MATRIX_H

#include <vector>
#include <cstdint>


#define TL std
using size = uint16_t;


template <size h, size w, typename scalar = float>
class Matrix;


template <size h, typename scalar = float>
using Vector = Matrix<h, 1, scalar>;


template <size h, size w, typename scalar>
class Matrix {
public:
	Matrix() = default;
	explicit Matrix(const TL::vector<scalar>& vector);
	explicit Matrix(const TL::vector<TL::vector<scalar>>& vector);
	Matrix(const TL::initializer_list<TL::initializer_list<scalar>>& list);

	template <size order>
	static Matrix<order, order> identity();

	Vector<w, scalar>& operator[](size i);
	const Vector<w, scalar>& operator[](size i) const;

	Matrix<w, h> transpose() const;
	Matrix invert() const;

	Matrix operator*(scalar multiplier) const;
	Matrix& operator*=(scalar multiplier);
	Matrix multiplyComponents(const Matrix& matrix) const;

	Matrix operator/(scalar divisor) const;
	Matrix& operator/=(scalar divisor);

	Matrix operator+(const Matrix& matrix) const;
	Matrix operator-(const Matrix& matrix) const;

	template <size oh, size ow>
	Matrix<h, ow> operator*(const Matrix<oh, ow>& matrix) const;

	template <size oh, size ow>
	Matrix<h, w + ow> concat(const Matrix<oh, ow>& matrix) const;

	size getWidth() const;
	size getHeight() const;

protected:
	scalar _values[h][w] {};
};


template <size h, size w, typename scalar>
Matrix<h, w, scalar>::Matrix(const TL::initializer_list<TL::initializer_list<scalar>>& values) {
	auto ri = values.begin();
	auto ei = ri->begin();

	for (size j {0}; j < h; ++j) {
		for (size i {0}; i < w; ++i) {
			_values[j][i] = *(ei++);
		}

		++ri;
	}
}


//template <size h, size w, typename scalar>
//Matrix<h, w, scalar>::Matrix(const TL::vector<scalar>& vector):
//		_w {1}, _h {static_cast<size>(vector.size())} {
//	for (size i {0}; i < vector.size(); ++i) {
//		_values.push_back(TL::vector<scalar> {vector[i]});
//	}
//}
//
//
//template <size h, size w, typename scalar>
//Matrix::Matrix(const TL::vector<TL::vector<scalar>>& vector):
//		_w {static_cast<size>(vector.front().size())}, _h {static_cast<size>(vector.size())} {
//	for (size i {0}; i < vector.size(); ++i) {
//		_values.emplace_back(vector[i]);
//	}
//}


template <size h, size w, typename scalar>
template <size order>
Matrix<order, order> Matrix<h, w, scalar>::identity() {
	Matrix<order, order> result {};

	for (size i {0}; i < order; ++i) {
		result[i][i] = 1;
	}

	return result;
}


template <size h, size w, typename scalar>
Vector<w, scalar>& Matrix<h, w, scalar>::operator[](size i) {
	return _values[i];
}


template <size h, size w, typename scalar>
const Vector<w, scalar>& Matrix<h, w, scalar>::operator[](size i) const {
	return _values[i];
}


template <size h, size w, typename scalar>
Matrix<w, h> Matrix<h, w, scalar>::transpose() const {
	Matrix<w, h> result {};

	for (size j {0}; j < h; ++j) {
		for (size i {0}; i < w; ++i) {
			result[i][j] = _values[j][i];
		}
	}
	return result;
}


template <size h, size w, typename scalar>
Matrix<h, w, scalar> Matrix<h, w, scalar>::invert() const {
	Matrix temp {*this};
	Matrix augmented {Matrix::identity<w>()};

	// Gaussian elimination
	for (size r1 {0}; r1 < w; ++r1) {
		for (size r2 {0}; r2 < w; ++r2) {
			if (r1 == r2) {
				continue;
			}

			scalar factor {temp[r2][r1] / temp[r1][r1]};

			for (size i {0}; i < w; ++i) {
				temp[r2][i] -= factor * temp[r1][i];
				augmented[r2][i] -= factor * augmented[r1][i];
			}
		}
	}

	// Gaining identity matrix
	for (size r {0}; r < w; ++r) {
		scalar factor = 1 / temp[r][r];

		for (size i {0}; i < w; ++i) {
			augmented[r][i] *= factor;
		}
	}

	return augmented;
}


template <size h, size w, typename scalar>
Matrix<h, w, scalar> Matrix<h, w, scalar>::operator*(scalar s) const {
	Matrix result {w, h};

	for (size j {0}; j < h; ++j) {
		for (size i {0}; i < w; ++i) {
			result[j][i] = _values[j][i] * s;
		}
	}
	return result;
}


template <size h, size w, typename scalar>
Matrix<h, w, scalar> Matrix<h, w, scalar>::operator/(scalar s) const {
	Matrix result {w, h};

	for (size j {0}; j < h; ++j) {
		for (size i {0}; i < w; ++i) {
			result[j][i] = _values[j][i] / s;
		}
	}
	return result;
}


template <size h, size w, typename scalar>
Matrix<h, w, scalar>& Matrix<h, w, scalar>::operator/=(scalar s) {
	for (size j {0}; j < h; ++j) {
		for (size i {0}; i < w; ++i) {
			_values[j][i] /= s;
		}
	}
	return *this;
}


template <size h, size w, typename scalar>
template <size oh, size ow>
Matrix<h, ow> Matrix<h, w, scalar>::operator*(const Matrix<oh, ow>& matrix) const {
	if (matrix._w == 1 && matrix._h == 1) {
		return operator*(matrix[0][0]);
	} else if (w != matrix._h) {
		return *this;
	}

	Matrix result {matrix._w, h};
	for (size j {0}; j < matrix._w; ++j) {
		for (size i {0}; i < h; ++i) {
			scalar sum {0};
			for (size k {0}; k < w; ++k) {
				sum += _values[i][k] * matrix._values[k][j];
			}
			result[i][j] = sum;
		}
	}
	return result;
}


template <size h, size w, typename scalar>
Matrix<h, w, scalar>& Matrix<h, w, scalar>::operator*=(scalar s) {
	for (size j {0}; j < h; ++j) {
		for (size i {0}; i < w; ++i) {
			_values[j][i] = _values[j][i] * s;
		}
	}
	return *this;
}


template <size h, size w, typename scalar>
Matrix<h, w, scalar> Matrix<h, w, scalar>::operator+(const Matrix& matrix) const {
	if (w != matrix._w || h != matrix._h) {
		return *this;
	}

	Matrix<w, h> result {};

	for (size j {0}; j < h; ++j) {
		for (size i {0}; i < w; ++i) {
			result[j][i] = _values[j][i] + matrix._values[j][i];
		}
	}
	return result;
}


template <size h, size w, typename scalar>
Matrix<h, w, scalar> Matrix<h, w, scalar>::operator-(const Matrix& matrix) const {
	if (w != matrix._w || h != matrix._h) {
		return *this;
	}

	Matrix<h, w, scalar> result {};

	for (size j {0}; j < h; ++j) {
		for (size i {0}; i < w; ++i) {
			result[j][i] = _values[j][i] - matrix._values[j][i];
		}
	}
	return result;
}


template <size h, size w, typename scalar>
Matrix<h, w, scalar> Matrix<h, w, scalar>::multiplyComponents(const Matrix& matrix) const {
	if (w != matrix._w || h != matrix._h) {
		return *this;
	}

	Matrix<h, w, scalar> result {};

	for (size j {0}; j < h; ++j) {
		for (size i {0}; i < w; ++i) {
			result[j][i] = _values[j][i] * matrix._values[j][i];
		}
	}
	return result;
}


template <size h, size w, typename scalar>
template <size oh, size ow>
Matrix<h, w + ow> Matrix<h, w, scalar>::concat(const Matrix<oh, ow>& matrix) const {
	if (h != matrix._h) {
		return *this;
	}

	Matrix<h, w + ow> result {};

	for (size j {0}; j < h; ++j) {
		for (size i {0}; i < w; ++i) {
			result[j][i] = _values[j][i];
		}
		for (size i {0}; i < matrix._w; ++i) {
			result[j][i + w] = matrix._values[j][i];
		}
	}
	return result;
}


template <size h, size w, typename scalar>
size Matrix<h, w, scalar>::getWidth() const {
	return w;
}


template <size h, size w, typename scalar>
size Matrix<h, w, scalar>::getHeight() const {
	return h;
}


#endif //TRADER_MATRIX_H
