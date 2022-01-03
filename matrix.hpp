#include <iostream>
#include <string>
#include <cmath>


bool is_equal(const double a, const double b)
{
	static const double epsilon = 1e-6;

	return std::fabs(a - b) <= epsilon;
}

namespace matrix
{
	template <typename Elem>
	class Row
	{
	private:
		Elem* pnt{nullptr};
		size_t r_size{0};

	public:
		Row() = default;

		explicit Row(size_t r_size): r_size{r_size}, pnt { new Elem[r_size]{} } {}

		Row(const Row<Elem>& other) = delete;
		Row<Elem>& operator= (const Row<Elem>& other) = delete;

		Row(Row<Elem>&& other) noexcept: 
			r_size{other.r_size}, pnt{other.pnt} { other.pnt = nullptr; }

		Row<Elem>& operator= (Row&& other) noexcept
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

		Row<Elem> operator* (const Elem& elem) const
		{
			Row<Elem> row{size()};

			for (size_t i = 0; i < size(); i++)
				row[i] = pnt[i] * elem;
		
			return row;
		}

		Row<Elem> operator/ (const Elem& elem) const
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
	protected:
		Row<Row<Elem>> elem;

	public:
		Matrix(size_t row = 0, size_t column = 0): elem{row}
		{
			if (column == 0) 
				column = row;

			for (size_t i = 0; i < row; i++)
				elem[i] = Row<Elem>{column};
		}

		Matrix(const Matrix<Elem>& other) { *this = other; }

		Matrix(Matrix<Elem>&& other): elem{other.elem} {}

		Matrix<Elem>& operator= (const Matrix<Elem>& other)
		{
			if (this != &other)
			{
				elem = Row<Row<Elem>>{other.size()};

				for (size_t i = 0; i < other.size(); i++)
				{
					elem[i] = Row<Elem>{other[i].size()};
					for (size_t j = 0; j < other[i].size(); j++)
						elem[i][j] = other[i][j];
				}
			}

			return *this;
		}

		Matrix<Elem>& operator= (Matrix<Elem>&& other)
		{
			if (this != &other)
				elem = std::move(other.elem);

			return *this;
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
					if ((!is_equal(elem[j][i], Elem{0})) &&
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

		Elem Determinant()
		{
			Matrix<double> matrix{*this};

			Elem det = Gauss();

			return det;
		}

		size_t size() const { return elem.size(); }

		Row<Elem>& operator[] (size_t it) const { return elem[it]; }

		virtual ~Matrix() {};
	};

	template <typename Elem>
	void Fill(std::istream& input, const Matrix<Elem>& matrix)
	{
		for (size_t i = 0; i < matrix.size(); i++)
		for (size_t j = 0; j < matrix[i].size(); j++)
			input >> matrix[i][j];
	}


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

}