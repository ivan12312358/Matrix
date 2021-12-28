#include <iostream>
#include <string>
#include <cmath>


template <typename Elem>
bool is_equal(const Elem& a, const Elem& b)
{
	static const double epsilon = 1e-6;

	if (a > b) return a - b <= epsilon;

	return b - a <= epsilon;
}

template <typename Elem>
class Row
{
private:
	Elem* pnt;
	size_t r_size;

public:
	Row(): r_size{0}, pnt{nullptr} {}

	Row(size_t r_size): r_size{r_size}, pnt { new Elem[r_size]{} } {}

	Row(const Row<Elem>& other) = delete;
	Row<Elem>& operator= (const Row<Elem>& other) = delete;

	Row(Row<Elem>&& other): 
		r_size{other.r_size}, pnt{other.pnt} { other.pnt = nullptr; }

	Row<Elem>& operator= (Row&& other)
	{
		if (this != &other)
		{
			delete[] pnt;
			r_size = other.r_size;
			pnt    = other.pnt;
			other.pnt = nullptr;
		}
		return *this;
	}

	Row<Elem>& operator-= (const Row<Elem>& other)
	{
		for (size_t i = 0; i < r_size; i++)
			pnt[i] = pnt[i] - other.pnt[i];

		return *this;
	}

	Row<Elem>& operator+= (const Row<Elem>& other)
	{
		for (size_t i = 0; i < r_size; i++)
			pnt[i] = pnt[i] + other.pnt[i];

		return *this;
	}

	Row<Elem>& operator/= (const Elem& elem)
	{
		for (size_t i = 0; i < size(); i++)
			pnt[i] = pnt[i] / elem;
	
		return *this;
	}

	Row<Elem> operator* (const Elem& elem)
	{
		Row<Elem> row{size()};

		for (size_t i = 0; i < size(); i++)
			row[i] = pnt[i] * elem;
	
		return row;
	}

	Row<Elem> operator/ (const Elem& elem)
	{
		Row<Elem> row{size()};

		for (size_t i = 0; i < size(); i++)
			row[i] = pnt[i] / elem;
	
		return row;
	}

	size_t size() const { return r_size; }

	Elem& operator[] (size_t it) const { return pnt[it]; }

	~Row() { delete[] pnt; }
};

template <typename Elem>
class Matrix
{
	Row<Row<Elem>> elem;

public:
	Matrix(size_t size = 0): elem{size} 
	{
		for(size_t i = 0; i < elem.size(); i++)
			elem[i] = Row<Elem>{size};
	}

	void Fill()
	{
		for (size_t i = 0; i < elem.size(); i++)
		for (size_t j = 0; j < elem[i].size(); j++)
			std::cin >> elem[i][j];
	}

	Elem Gauss()
	{
		Elem pivot, multipliers{1};
		int pos;

		for (size_t i = 0; i < size(); i++)
		{
			pivot =  0;
			pos   = -1;

			for (size_t j = i; j < size(); j++)
			{
				if ((elem[j][i] != Elem{0}) &&
					(pivot < std::fabs(elem[j][i]) || j == i))
				{
					pivot = std::fabs(elem[j][i]);
					pos = j;
				}
			}

			if (pos == -1)
				return 0;

			pivot = elem[pos][i];
			multipliers *= pivot;
			elem[pos] 	/= pivot;

			if (pos != i)
			{
				std::swap(elem[pos], elem[i]);
				multipliers *= -1;
			}

			for (size_t j = i + 1; j < size(); j++)
				elem[j] -= elem[i] * elem[j][i];
		}

		return multipliers;
	}

	Elem Determinant() const
	{
		Matrix<double> matrix{size()};

		for (size_t i = 0; i < elem.size(); i++)
		for (size_t j = 0; j < elem[i].size(); j++)
			matrix[i][j] = elem[i][j];

		Elem det = matrix.Gauss();

		return det;
	}

	size_t size() const { return elem.size(); }

	Row<Elem>& operator[] (size_t it) const { return elem[it]; }
};

template <typename Elem>
std::ostream& operator << (std::ostream& out, const Matrix<Elem>& matrix)
{
	for (size_t i = 0; i < matrix.size(); i++)
	{
		for (size_t j = 0; j < matrix.size(); j++)
			out << matrix[i][j] << " ";

		out << std::endl;
	}
	return out;
}
