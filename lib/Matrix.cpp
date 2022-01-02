//
// Created by MStefan99 on 23.7.21.
//

#include "Matrix.h"


Matrix::Matrix(size_t w, size_t h):
		_w {w}, _h {h} {
	_values.resize(_h);
	for (size_t j {0}; j < _h; ++j) {
		_values[j].resize(_w);
	}
}


void Matrix::randomize() {
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<scalar> distribution {-.1, .1};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			_values[j][i] = distribution(generator);
		}
	}
}


Matrix::Matrix(const std::initializer_list<std::initializer_list<scalar>>& values):
		_w {values.begin()->size()}, _h {values.size()} {
	for (const auto& row : values) {
		std::vector<scalar> v {};
		for (auto value: row) {
			v.push_back(value);
		}
		_values.push_back(v);
	}
}


Matrix::Matrix(const std::vector<scalar>& vector):
		_w {1}, _h {vector.size()} {
	for (size_t i {0}; i < vector.size(); ++i) {
		_values.push_back(std::vector<scalar> {vector[i]});
	}
}


Matrix::Matrix(const std::vector<std::vector<scalar>>& vector):
		_w {vector.front().size()}, _h {vector.size()} {
	for (size_t i {0}; i < vector.size(); ++i) {
		_values.emplace_back(vector[i]);
	}
}


Matrix Matrix::identity(size_t order) {
	Matrix result {order, order};

	for (size_t i {0}; i < order; ++i) {
		result[i][i] = 1;
	}

	return result;
}


std::vector<scalar>& Matrix::operator[](size_t i) {
	return _values[i];
}


const std::vector<scalar>& Matrix::operator[](size_t i) const {
	return _values[i];
}


Matrix Matrix::transpose() const {
	Matrix result {_h, _w};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[i][j] = _values[j][i];
		}
	}
	return result;
}


Matrix Matrix::invert() const {
	if (_w != _h) {
		throw std::length_error("Matrix not square");
	}

	Matrix temp {*this};
	Matrix augmented {Matrix::identity(_w)};

	// Gaussian elimination
	for (size_t r1 {0}; r1 < _w; ++r1) {
		for (size_t r2 {0}; r2 < _w; ++r2) {
			if (r1 == r2) {
				continue;
			}

			scalar factor {temp[r2][r1] / temp[r1][r1]};

			for (size_t i {0}; i < _w; ++i) {
				temp[r2][i] -= factor * temp[r1][i];
				augmented[r2][i] -= factor * augmented[r1][i];
			}
		}
	}

	// Gaining identity matrix
	for (size_t r {0}; r < _w; ++r) {
		scalar factor = 1 / temp[r][r];

		for (size_t i {0}; i < _w; ++i) {
			augmented[r][i] *= factor;
		}
	}

	return augmented;
}


Matrix Matrix::operator*(scalar scalar) const {
	Matrix result {_w, _h};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] * scalar;
		}
	}
	return result;
}


Matrix Matrix::operator/(scalar scalar) const {
	Matrix result {_w, _h};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] / scalar;
		}
	}
	return result;
}


Matrix& Matrix::operator/=(scalar scalar) {
	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			_values[j][i] /= scalar;
		}
	}
	return *this;
}


Matrix Matrix::operator*(const Matrix& matrix) const {
	if (matrix._w == 1 && matrix._h == 1) {
		return operator*(matrix[0][0]);
	} else if (_w != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {matrix._w, _h};
	for (size_t j {0}; j < matrix._w; ++j) {
		for (size_t i {0}; i < _h; ++i) {
			scalar sum {0};
			for (size_t k {0}; k < _w; ++k) {
				sum += _values[i][k] * matrix._values[k][j];
			}
			result[i][j] = sum;
		}
	}
	return result;
}


Matrix& Matrix::operator*=(scalar scalar) {
	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			_values[j][i] = _values[j][i] * scalar;
		}
	}
	return *this;
}


Matrix Matrix::operator+(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {_w, _h};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] + matrix._values[j][i];
		}
	}
	return result;
}


Matrix Matrix::operator-(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {_w, _h};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] - matrix._values[j][i];
		}
	}
	return result;
}


Matrix Matrix::multiplyComponents(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {_w, _h};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] * matrix._values[j][i];
		}
	}
	return result;
}


Matrix Matrix::concat(const Matrix& matrix) const {
	if (_h != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {_w + matrix._w, _h};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i];
		}
		for (size_t i {0}; i < matrix._w; ++i) {
			result[j][i + _w] = matrix._values[j][i];
		}
	}
	return result;
}


Matrix::operator std::vector<scalar>() const {
	if (_w != 1) {
		throw std::length_error("Matrix dimension mismatch");
	}

	std::vector<scalar> result {};

	for (const auto& v: _values) {
		result.push_back(v[0]);
	}
	return result;
}


Matrix::operator std::vector<std::vector<scalar>>() const {
	return _values;
}


size_t Matrix::getWidth() const {
	return _w;
}


size_t Matrix::getHeight() const {
	return _h;
}


std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
	out << matrix._w << ':' << matrix._h << std::endl;

	for (size_t j {0}; j < matrix._h; ++j) {
		for (size_t i {0}; i < matrix._w; ++i) {
			if (i) {
				out << ',';
			}
			out << matrix[j][i];
		}
		out << std::endl;
	}

	return out;
}


std::istream& operator>>(std::istream& in, Matrix& matrix) {
	in >> matrix._w >> ':' >> matrix._h;

	matrix._values.resize(matrix._h);

	for (size_t j {0}; j < matrix._h; ++j) {
		matrix._values[j].resize(matrix._w);

		for (size_t i {0}; i < matrix._w; ++i) {
			if (i) {
				in >> ',';
			}
			in >> matrix[j][i];
		}
	}

	return in;
}
