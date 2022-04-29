//
// Created by MStefan99 on 23.7.21.
//

#include "Matrix.h"


Matrix::Matrix(size_type w, size_type h):
		_w {w}, _h {h} {
	_values = allocator_type().allocate(_h * _w);
}


Matrix::Matrix(const std::initializer_list<std::initializer_list<scalar>>& values):
		_w {static_cast<size_type>(values.begin()->size())}, _h {static_cast<size_type>(values.size())} {
	_values = allocator_type().allocate(_h * _w);

	size_type j = 0;
	for (auto& row : values) {
		size_type i = 0;
		for (auto& e : row) {
			this->operator[](j)[i++] = e;
		}
		++j;
	}
}


Matrix::Matrix(const Matrix& matrix): Matrix(matrix._w, matrix._h) {
	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			this->operator[](j)[i] = matrix[j][i];
		}
	}
}


Matrix& Matrix::operator=(const Matrix& matrix) {
	if (this != &matrix) {
		allocator_type().deallocate(_values, _h * _w);

		_h = matrix._h;
		_w = matrix._w;
		_values = allocator_type().allocate(_h * _w);

		for (size_type j {0}; j < _h; ++j) {
			for (size_type i {0}; i < _w; ++i) {
				this->operator[](j)[i] = matrix[j][i];
			}
		}
	}
	return *this;
}


Matrix::~Matrix() {
	allocator_type().deallocate(reinterpret_cast<scalar*>(_values), _w * _h);
}


Matrix Matrix::identity(size_type order) {
	Matrix result {order, order};

	for (size_type i {0}; i < order; ++i) {
		result[i][i] = 1;
	}

	return result;
}


typename Matrix::scalar* Matrix::operator[](size_type i) {
	return _values + (i * _w);
}


const typename Matrix::scalar* Matrix::operator[](size_type i) const {
	return _values + (i * _w);
}


Matrix Matrix::transpose() const {
	Matrix result {_h, _w};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[i][j] = this->operator[](j)[i];
		}
	}
	return result;
}


Matrix Matrix::invert() const {
	if (_w != _h) {
		return *this;
	}

	Matrix temp {*this};
	Matrix augmented {Matrix::identity(_w)};

	// Gaussian elimination
	for (size_type r1 {0}; r1 < _w; ++r1) {
		for (size_type r2 {0}; r2 < _w; ++r2) {
			if (r1 == r2) {
				continue;
			}

			scalar factor {temp[r2][r1] / temp[r1][r1]};

			for (size_type i {0}; i < _w; ++i) {
				temp[r2][i] -= factor * temp[r1][i];
				augmented[r2][i] -= factor * augmented[r1][i];
			}
		}
	}

	// Gaining identity matrix
	for (size_type r {0}; r < _w; ++r) {
		scalar factor = 1 / temp[r][r];

		for (size_type i {0}; i < _w; ++i) {
			augmented[r][i] *= factor;
		}
	}

	return augmented;
}


Matrix Matrix::operator*(scalar scalar) const {
	Matrix result {_w, _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i] * scalar;
		}
	}
	return result;
}


Matrix Matrix::operator/(scalar scalar) const {
	Matrix result {_w, _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i] / scalar;
		}
	}
	return result;
}


Matrix& Matrix::operator/=(scalar scalar) {
	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			this->operator[](j)[i] /= scalar;
		}
	}
	return *this;
}


Matrix Matrix::operator*(const Matrix& matrix) const {
	if (matrix._w == 1 && matrix._h == 1) {
		return operator*(matrix[0][0]);
	} else if (_w != matrix._h) {
		return *this;
	}

	Matrix result {matrix._w, _h};
	for (size_type j {0}; j < matrix._w; ++j) {
		for (size_type i {0}; i < _h; ++i) {
			scalar sum {0};
			for (size_type k {0}; k < _w; ++k) {
				sum += this->operator[](i)[k] * matrix[k][j];
			}
			result[i][j] = sum;
		}
	}
	return result;
}


Matrix& Matrix::operator*=(scalar scalar) {
	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			this->operator[](j)[i] = this->operator[](j)[i] * scalar;
		}
	}
	return *this;
}


Matrix Matrix::operator+(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		return *this;
	}

	Matrix result {_w, _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i] + matrix[j][i];
		}
	}
	return result;
}


Matrix Matrix::operator-(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		return *this;
	}

	Matrix result {_w, _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i] - matrix[j][i];
		}
	}
	return result;
}


Matrix Matrix::multiplyComponents(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		return *this;
	}

	Matrix result {_w, _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i] * matrix[j][i];
		}
	}
	return result;
}


Matrix Matrix::concat(const Matrix& matrix) const {
	if (_h != matrix._h) {
		return *this;
	}

	Matrix result {static_cast<size_type>(_w + matrix._w), _h};

	for (size_type j {0}; j < _h; ++j) {
		for (size_type i {0}; i < _w; ++i) {
			result[j][i] = this->operator[](j)[i];
		}
		for (size_type i {0}; i < matrix._w; ++i) {
			result[j][i + _w] = matrix[j][i];
		}
	}
	return result;
}


typename Matrix::size_type Matrix::getWidth() const {
	return _w;
}


typename Matrix::size_type Matrix::getHeight() const {
	return _h;
}


#ifdef MATRIX_IO


std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
	out << matrix._w << ':' << matrix._h << std::endl;

	for (typename Matrix::size_type j {0}; j < matrix._h; ++j) {
		for (typename Matrix::size_type i {0}; i < matrix._w; ++i) {
			if (i) {
				out << ',';
			}
			out << matrix[j][i];
		}
		out << std::endl;
	}

	return out;
}


#endif
