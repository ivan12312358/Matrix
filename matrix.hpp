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
		size_t r_size{0};
		Elem* pnt{nullptr};

	public:
		Row<Elem>() = default;

		explicit Row<Elem>(size_t r_size): r_size{r_size}, pnt { new Elem[r_size]{} } {}

		Row<Elem>(const Row<Elem>& other)
		{
			Row<Elem> tmp(other.size());

			for (size_t i = 0; i < tmp.r_size; ++i)
				tmp.pnt[i] = other.pnt[i];

			std::swap(tmp, *this);	
		}

		Row<Elem>(Row<Elem>&& other) noexcept 
		{
			std::swap(r_size, other.r_size);
			std::swap(pnt, other.pnt);
		}

		Row<Elem>& operator = (const Row<Elem>& other)
		{
			if (this != &other)
			{
				Row<Elem> tmp(other);
				std::swap(tmp, *this);
			}
			return *this;
		}

		Row<Elem>& operator = (Row<Elem>&& other) noexcept
		{
			if (this != &other)
			{
				std::swap(r_size, other.r_size);
				std::swap(pnt, other.pnt);
			}
			return *this;
		}

		Row<Elem>& operator -= (const Row<Elem>& other)
		{
			for (size_t i = 0; i < r_size; i++)
				pnt[i] = pnt[i] - other.pnt[i];

			return *this;
		}

		Row<Elem>& operator += (const Row<Elem>& other)
		{
			for (size_t i = 0; i < r_size; i++)
				pnt[i] += other.pnt[i];

			return *this;
		}

		Row<Elem>& operator /= (const Elem& elem)
		{
			for (size_t i = 0; i < size(); i++)
				pnt[i] = pnt[i] / elem;
		
			return *this;
		}

		Row<Elem>& operator *= (const Elem& elem)
		{
			for (size_t i = 0; i < size(); i++)
				pnt[i] *= elem;
		
			return *this;
		}

		Row<Elem> operator - (const Row<Elem>& other) const
		{
			Row<Elem> row{size()};
			row -= other;
			return row;
		}

		Row<Elem> operator + (const Row<Elem>& other) const
		{
			Row<Elem> row{size()};
			row += other;
			return row;
		}

		Row<Elem> operator * (const Elem& elem) const
		{
			Row<Elem> row{size()};
			row *= elem;
			return row;
		}

		Row<Elem> operator / (const Elem& elem) const
		{
			Row<Elem> row{size()};
			row /= elem;		
			return row;
		}

		bool operator == (const Row<Elem>& other) const { return pnt == other.pnt; }

		bool is_same(const Row<Elem>& other) const
		{
			if (r_size != other.r_size)
				return false;

			for (size_t it = 0; it < r_size; ++it)
				if (pnt[it] != other.pnt[it])
					return false;

			return true;
		}

		size_t size() const { return r_size; }

		Elem& operator[] (size_t it) const { return pnt[it]; }

		~Row() { delete[] pnt; }

		void novalid() { pnt = nullptr; r_size = 0; }

		void lazycopy(const Row<Elem>& other)
		{ 
			r_size = other.r_size;
			pnt    = other.pnt;
		}
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

			for (size_t i = 0; i < other.size(); ++i)
			for (size_t j = 0; j < other[i].size(); ++j)
				tmp[i][j] = other[i][j];

			std::swap(tmp, *this);
		}

		Matrix(const Matrix<Elem>& other) noexcept = default;

		Matrix(Matrix<Elem>&& other) noexcept = default;

		Matrix<Elem>& operator= (const Matrix<Elem>& other) noexcept = default;

		Matrix<Elem>& operator= (Matrix<Elem>&& other) noexcept = default;

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
	class LazyMatrix final: public Matrix<Elem>
	{
		int* copies{nullptr};
	public:
		enum class equivalence
		{
			equal_buf,
			equal_elems,
			not_equal
		};

		using Matrix<Elem>::elem;

		LazyMatrix(size_t row = 0): Matrix<Elem> {row}, copies{new int{0}} {}

		LazyMatrix(size_t row, Elem el): LazyMatrix<Elem> {row} { Fill(el, *this); }

		LazyMatrix(const LazyMatrix<Elem>& other) { *this = other; }

		LazyMatrix(LazyMatrix<Elem>&& other) { *this = std::move(other); }

		LazyMatrix<Elem>& operator= (const LazyMatrix<Elem>& other)
		{
		   	if ((this != &other) && (copies != other.copies))
			{
				Row<Row<Elem>> tmp;
				tmp.lazycopy(other.elem);

				if (copies && (*copies != 0))
				{
					(*copies)--;
					elem.novalid();
				}
				else delete copies;

				std::swap(tmp, elem);
				copies = other.copies;
				(*copies)++;
			}
			return *this;
		}

		LazyMatrix<Elem>& operator= (LazyMatrix<Elem>&& other)
		{
			if ((this != &other) && (copies != other.copies))
			{
				if (copies && (*copies != 0))
				{
					(*copies)--;
					elem.novalid();
					copies = nullptr;
				}

				elem = std::move(other.elem);
				std::swap(copies, other.copies);
			}
			return *this;
		}

		LazyMatrix& operator += (const LazyMatrix<Elem>& other)
		{
			if (elem.size() == other.size())
			{
				deepcopy();

				for (size_t i = 0; i < elem.size(); ++i)
					elem[i] += other[i];
			}
			return *this;			

		}

		LazyMatrix& operator -= (const LazyMatrix<Elem>& other)
		{
			if (elem.size() == other.size())
			{
				deepcopy();

				for (size_t i = 0; i < elem.size(); ++i)
					elem[i] -= other[i];
			}
			return *this;			
		}

		LazyMatrix& operator *= (const Elem& other)
		{
			deepcopy();

			for (size_t i = 0; i < elem.size(); ++i)
				elem[i] *= other;

			return *this;			
		}

		LazyMatrix& operator /= (const Elem& other)
		{
			deepcopy();

			for (size_t i = 0; i < elem.size(); ++i)
				elem[i] /= other;

			return *this;			
		}

		LazyMatrix operator + (const LazyMatrix<Elem>& other) const
		{
			LazyMatrix tmp{*this};
			tmp += other;
			return tmp;
		}

		LazyMatrix operator - (const LazyMatrix<Elem>& other) const
		{
			LazyMatrix tmp{*this};
			tmp -= other;
			return tmp;
		}

		LazyMatrix operator * (const Elem& other) const
		{
			LazyMatrix tmp{*this};
			tmp *= other;
			return tmp;
		}

		LazyMatrix operator / (const Elem& other) const
		{
			LazyMatrix tmp{*this};
			tmp /= other;
			return tmp;
		}

		equivalence operator == (const LazyMatrix<Elem>& other) 
		{
			if (this->size() == other.size())
			{
				if (elem == other.elem)
					return equivalence::equal_buf;

				for (size_t i = 0; i < other.size(); ++i)
				for (size_t j = 0; j < other.size(); ++j)
					if (elem[i][j] != other.elem[i][j])
						return equivalence::not_equal;

				return equivalence::equal_elems;
			}			
			
			return equivalence::not_equal;
		}

		~LazyMatrix()
		{
			if (copies && (*copies != 0))
			{
				(*copies)--;
				elem.novalid();
				return;
			}
			delete copies;
		}
	private:
		void deepcopy()
		{
			LazyMatrix<Elem> tmp(elem.size());

			for (size_t i = 0; i < elem.size(); ++i)
			for (size_t j = 0; j < elem[i].size(); ++j)
				tmp[i][j] = elem[i][j];

			std::swap(tmp, *this);
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
	void Fill(Elem elem, const Matrix<Elem>& matrix)
	{
		for (size_t i = 0; i < matrix.size(); i++)
		for (size_t j = 0; j < matrix[i].size(); j++)
			matrix[i][j] = elem;
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