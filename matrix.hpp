#include <iostream>
#include <string>
#include <cmath>
#include <exception>

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
		size_t r_size{0};
		Elem* pnt{nullptr};

	public:
		Row<Elem>() = default;

		explicit Row<Elem>(size_t r_size): r_size{r_size}, pnt { new Elem[r_size]{} } {}

		Row<Elem>(const Row<Elem>& other) noexcept { *this = other; } 

		Row<Elem>& operator= (const Row<Elem>& other) noexcept
		{
			if (this != &other)
			{
				try {
					Row<Elem> tmp{other.size()};

					for (size_t i = 0; i < tmp.r_size; ++i)
						tmp.pnt[i] = other.pnt[i];

					std::swap(tmp, *this);	
				} catch(const std::exception& e) {
					std::cerr << e.what() << '\n';
				}
			}
			return *this;
		}

		Row<Elem>(Row<Elem>&& other) noexcept: 
			r_size{other.r_size}, pnt{other.pnt} { other.pnt = nullptr; }

		Row<Elem>& operator= (Row<Elem>&& other) noexcept
		{
			if (this != &other)
			{
				r_size = other.r_size;
				std::swap(pnt, other.pnt);
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

		void novalid() { pnt = nullptr; }

		~Row() { delete[] pnt; }
	};

	template <typename Elem>
	class Matrix
	{
	protected:
		Row<Row<Elem>> elem;
	public:
		Matrix(size_t row = 0): elem{row}
		{
			for (size_t i = 0; i < row; i++)
				elem[i] = Row<Elem>{row};
		}

		template <typename T>
		Matrix(const Matrix<T>& other)
		{
			Matrix<Elem> tmp{other.size()};

			for (size_t i = 0; i < other.size(); i++)
			for (size_t j = 0; j < other[i].size(); j++)
				tmp[i][j] = other[i][j];

			std::swap(tmp, *this);
		}

		Matrix(const Matrix<Elem>& other) = default;

		Matrix(Matrix<Elem>&& other) = default;

		Matrix<Elem>& operator= (const Matrix<Elem>& other) noexcept
		{
			if (this != &other)
				elem = other.elem;

			return *this;
		}

		Matrix<Elem>& operator= (Matrix<Elem>&& other) noexcept
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

		Elem Determinant() const
		{
			Matrix<double> matrix{*this};

			Elem det = matrix.Gauss();

			return det;
		}

		size_t size() const { return elem.size(); }

		Row<Elem>& operator[] (size_t it) const { return elem[it]; }

		virtual ~Matrix() {};
	};

	template <typename Elem>
	class LazyMatrix: Matrix<Elem>
	{
	public:
		int* copies;
	public:
		using Matrix<Elem>::elem;

		LazyMatrix(size_t row = 0, size_t column = 0): Matrix<Elem> {row, column}, copies{new int{0}} {}

		LazyMatrix(const LazyMatrix<Elem>& other) { *this = other; }

		LazyMatrix(LazyMatrix<Elem>&& other) { *this = std::move(other); }

		LazyMatrix<Elem>& operator= (const LazyMatrix<Elem>& other)
		{
			std::cout << "asdasd\n";

			if (this != &other)
			{
				if (copies == nullptr) 
				{ ; }
				else if (*copies == 0)
					delete copies;
				else
				{
					(*copies)--;
					elem.novalid();
				}

				elem   = other.elem;
				copies = other.copies;
				(*copies)++;
			}

			return *this;
		}

		LazyMatrix<Elem>& operator= (LazyMatrix<Elem>&& other)
		{
			if (this != &other)
			{
				if (copies == nullptr) 
				{ ; }
				else if (*copies == 0)
					delete copies;
				else
				{
					(*copies)--;
					elem.novalid();
				}

				elem   = std::move(other.elem);
				copies = other.copies;
				other.copies = nullptr;
			}

			return *this;
		}

		~LazyMatrix()
		{
			if (*copies != 0)
			{
				*(copies)--;
				elem.novalid();
				return;
			}

			delete copies;
		}
	};

	template <typename Elem>
	void Fill(std::istream& input, const Matrix<Elem>& matrix)
	{
		Elem elem{};

		for (size_t i = 0; i < matrix.size(); i++)
		for (size_t j = 0; j < matrix[i].size(); j++)
		{
			input >> elem;

			if (input.good())
				matrix[i][j] = elem;
			else
			{
				std::cerr << "bad input element\n";
				return;
			}
		}
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