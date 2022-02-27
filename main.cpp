#include "matrix.hpp"

int main()
{
	size_t size{0};
	std::cin >> size;

	matrix::Matrix<double> m{size};
	matrix::Fill(std::cin, m);

	std::cout << m.Determinant() << std::endl;

/* 	matrix::LazyMatrix<int> a{20};
	
	matrix::LazyMatrix<int> b{2};
	matrix::LazyMatrix<int> c{2};

	matrix::Fill(std::cin, b);
	matrix::Fill(std::cin, c);

	a = b;
	a += c;

	std::cout << a;
	std::cout << b;
 */
	return 0;
}