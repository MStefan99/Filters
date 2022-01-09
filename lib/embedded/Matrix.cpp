//
// Created by MStefan99 on 23.7.21.
//

#include "Matrix.h"


Matrix::Matrix(size w, size h):
		_w {w}, _h {h} {
	_values.resize(_h);
	for (size j {0}; j < _h; ++j) {
		_values[j].resize(_w);
	}
}


Matrix::Matrix(const std::initializer_list<std::initializer_list<scalar>>& values):
		_w {static_cast<size>(values.begin()->size())}, _h {static_cast<size>(values.size())} {
	for (const auto& row : values) {
		std::vector<scalar> v {};
		for (auto value: row) {
			v.push_back(value);
		}
		_values.push_back(v);
	}
}


Matrix::Matrix(const std::vector<scalar>& vector):
		_w {1}, _h {static_cast<size>(vector.size())} {
	for (size i {0}; i < vector.size(); ++i) {
		_values.push_back(std::vector<scalar> {vector[i]});
	}
}


Matrix::Matrix(const std::vector<std::vector<scalar>>& vector):
		_w {static_cast<size>(vector.front().size())}, _h {static_cast<size>(vector.size())} {
	for (size i {0}; i < vector.size(); ++i) {
		_values.emplace_back(vector[i]);
	}
}


Matrix Matrix::identity(size order) {
	Matrix result {order, order};

	for (size i {0}; i < order; ++i) {
		result[i][i] = 1;
	}

	return result;
}


std::vector<scalar>& Matrix::operator[](size i) {
	return _values[i];
}


const std::vector<scalar>& Matrix::operator[](size i) const {
	return _values[i];
}


Matrix Matrix::transpose() const {
	Matrix result {_h, _w};

	for (size j {0}; j < _h; ++j) {
		for (size i {0}; i < _w; ++i) {
			result[i][j] = _values[j][i];
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
	for (size r1 {0}; r1 < _w; ++r1) {
		for (size r2 {0}; r2 < _w; ++r2) {
			if (r1 == r2) {
				continue;
			}

			scalar factor {temp[r2][r1] / temp[r1][r1]};

			for (size i {0}; i < _w; ++i) {
				temp[r2][i] -= factor * temp[r1][i];
				augmented[r2][i] -= factor * augmented[r1][i];
			}
		}
	}

	// Gaining identity matrix
	for (size r {0}; r < _w; ++r) {
		scalar factor = 1 / temp[r][r];

		for (size i {0}; i < _w; ++i) {
			augmented[r][i] *= factor;
		}
	}

	return augmented;
}


Matrix Matrix::operator*(scalar scalar) const {
	Matrix result {_w, _h};

	for (size j {0}; j < _h; ++j) {
		for (size i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] * scalar;
		}
	}
	return result;
}


Matrix Matrix::operator/(scalar scalar) const {
	Matrix result {_w, _h};

	for (size j {0}; j < _h; ++j) {
		for (size i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] / scalar;
		}
	}
	return result;
}


Matrix& Matrix::operator/=(scalar scalar) {
	for (size j {0}; j < _h; ++j) {
		for (size i {0}; i < _w; ++i) {
			_values[j][i] /= scalar;
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
	for (size j {0}; j < matrix._w; ++j) {
		for (size i {0}; i < _h; ++i) {
			scalar sum {0};
			for (size k {0}; k < _w; ++k) {
				sum += _values[i][k] * matrix._values[k][j];
			}
			result[i][j] = sum;
		}
	}
	return result;
}


Matrix& Matrix::operator*=(scalar scalar) {
	for (size j {0}; j < _h; ++j) {
		for (size i {0}; i < _w; ++i) {
			_values[j][i] = _values[j][i] * scalar;
		}
	}
	return *this;
}


Matrix Matrix::operator+(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		return *this;
	}

	Matrix result {_w, _h};

	for (size j {0}; j < _h; ++j) {
		for (size i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] + matrix._values[j][i];
		}
	}
	return result;
}


Matrix Matrix::operator-(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		return *this;
	}

	Matrix result {_w, _h};

	for (size j {0}; j < _h; ++j) {
		for (size i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] - matrix._values[j][i];
		}
	}
	return result;
}


Matrix Matrix::multiplyComponents(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		return *this;
	}

	Matrix result {_w, _h};

	for (size j {0}; j < _h; ++j) {
		for (size i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] * matrix._values[j][i];
		}
	}
	return result;
}


Matrix Matrix::concat(const Matrix& matrix) const {
	if (_h != matrix._h) {
		return *this;
	}

	Matrix result {static_cast<size>(_w + matrix._w), _h};

	for (size j {0}; j < _h; ++j) {
		for (size i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i];
		}
		for (size i {0}; i < matrix._w; ++i) {
			result[j][i + _w] = matrix._values[j][i];
		}
	}
	return result;
}


Matrix::operator std::vector<scalar>() const {
	std::vector<scalar> result {};

	for (const auto& v: _values) {
		result.push_back(v[0]);
	}
	return result;
}


Matrix::operator std::vector<std::vector<scalar>>() const {
	return _values;
}


size Matrix::getWidth() const {
	return _w;
}


size Matrix::getHeight() const {
	return _h;
}
