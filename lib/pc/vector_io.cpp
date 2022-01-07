//
// Created by MStefan99 on 27.7.21.
//

#include "vector_io.h"


std::ostream& operator<<(std::ostream& out, const std::vector<scalar>& vector) {
	Matrix m {vector};
	out << m;
	return out;
}


std::istream& operator>>(std::istream& in, std::vector<scalar>& vector) {
	Matrix m {};
	in >> m;

	vector = static_cast<std::vector<scalar>>(m);
	return in;
}


std::ostream& operator<<(std::ostream& out, const std::vector<std::vector<scalar>>& vector) {
	Matrix m {vector};
	out << m;
	return out;
}


std::istream& operator>>(std::istream& in, std::vector<std::vector<scalar>>& vector) {
	Matrix m {};
	in >> m;

	vector = static_cast<std::vector<std::vector<scalar>>>(m);
	return in;
}
