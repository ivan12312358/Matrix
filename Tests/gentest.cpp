#include "../matrix.hpp"
#include <stdlib.h>
#include <fstream>
#include <ctime>
#include <set>

using m_type = int;

void RandFill(Matrix<m_type>& m);
void decomposition(long det, std::multiset<m_type>& multiplies, size_t m_size);

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Not enough arguments" << std::endl;
		return -1;
	}

	std::srand(std::time(nullptr));

	long determinant = rand() % 10000 - 5000;

	std::fstream test{argv[1], std::ios::out},
				 answ{argv[2], std::ios::out};

	size_t size{10};

	answ << determinant << std::endl;
	test << size << std::endl;
	
	Matrix<m_type> m{size};

	for (size_t i = 0; i < size; i++)
		m[i][i] = 1;

	std::multiset<m_type> multiplies{};

	decomposition(determinant, multiplies, m.size());

	size_t index{0};

	for (auto it = multiplies.cbegin(); it != multiplies.cend(); it++)
	{
		m[index][index] = *it;
		index++;
	}

	RandFill(m);

	test << m << std::endl;

	std::cout << m.Determinant() << std::endl;

	return 0;
}

void decomposition(long det, std::multiset<m_type>& multiplies, size_t m_size)
{
	m_type sqrt_det = std::sqrt(std::abs(det));

	for (int i = 2; i < sqrt_det + 1; i++)
	{
		if (det % i == 0)
		{
			multiplies.insert(i);
			det /= i;
			i--;
		}
	}

	if (det != 1) multiplies.insert(det);

	while (multiplies.size() > m_size)
	{
		m_type mult1 = *(multiplies.cbegin());
		multiplies.erase(multiplies.cbegin());

		m_type mult2 = *(multiplies.cbegin());
		multiplies.erase(multiplies.cbegin());

		multiplies.insert(mult1 * mult2);
	}
}

void RandFill(Matrix<m_type>& m)
{
	if (m.size() == 1)
		return;

	srand(time(nullptr));

	size_t row{}, swap_cnt{};

	for(int i = 0; i < m.size(); i++)
	{
		row = rand() % m.size();

		if(i != row) 
		{
			std::swap(m[i], m[row]);
			swap_cnt++;
		}
	}

	if(swap_cnt % 2 != 0)
		m[0] = m[0] * (-1);

	for (size_t i = 0; i < m.size(); i++)
	for (size_t j = 0; j < m.size(); j++)
	{
		int sign = rand() % 3 - 1;
		if(i != j) m[j] += m[i] * sign;
	}

	m_type multiplier{};

	for (size_t i = 0; i < m.size(); i++)
	{
		int sign = rand() % 3 - 1;
		multiplier = (rand() % sizeof(m_type) + 1) / 0.9;

		do 	row = rand() % m.size();
		while(row == i);

		m[i] += m[row] * (multiplier * sign);
	}
}
