#include "matrix.hpp"

int main()
{
	size_t size{0};
	std::cin >> size;

	matrix::LazyMatrix<double> m(size);
	matrix::Fill(std::cin, m);

	std::cout << m.Determinant() << std::endl;

	return 0;
}