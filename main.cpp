#include "matrix.hpp"

int main()
{
	size_t size{0};
	std::cin >> size;

	Matrix<double> m{size};
	m.Fill();

	std::cout << m.Determinant() << std::endl;

	return 0;
}