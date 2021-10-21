#include <iostream>
#include <string>
#include <cmath>


template <typename Elem>
bool is_equal(const Elem& a, const Elem& b)
{
	static const double epsilon = 1e-8;

	if (a > b) return a - b <= epsilon;

	return b - a <= epsilon;
}

template <typename Elem>
struct Fraction
{
	Elem numerator;
	Elem denominator;

	Fraction(Elem num = 0, Elem denom = 1): numerator{num}, denominator{denom} { Reduction(); }

	Fraction<Elem> operator* (const Fraction<Elem>& other) const
	{
		return Fraction<Elem> {numerator   * other.numerator, 
							   denominator * other.denominator};
	}

	Fraction<Elem> operator/ (const Fraction<Elem>& other) const
	{
		return Fraction<Elem> {numerator   * other.denominator,
							   denominator * other.numerator};
	}

	Fraction<Elem> operator- (const Fraction<Elem>& other) const
	{
		return Fraction<Elem> {numerator   * other.denominator -
							   denominator * other.numerator,
							   denominator * other.denominator};
	}

	bool operator== (const Fraction<Elem>& other) const
	{
		return is_equal<Elem>(numerator, other.numerator) && 
			   is_equal<Elem>(denominator, other.denominator);
	}

	bool operator== (const Elem& other) const
	{
		return *this == Fraction<Elem>(other);
	}

	void operator+= (const Fraction<Elem>& other)
	{
		numerator 	= numerator   * other.denominator + denominator * other.numerator;
		denominator = denominator * other.denominator;

		Reduction();
	}

	void Reduction()
	{
		if (is_equal<Elem>(numerator, Elem{0}) || is_equal<Elem>(denominator, Elem{0}))
			return;
		if (numerator < Elem{1} || denominator < Elem{1})
			return;

		Elem num{std::abs(numerator)}, denom{std::abs(denominator)};

		while (!is_equal<Elem>(num, denom))
		{
			if (num > denom) 	  
				num -= denom;
			else if (denom > num) 
				denom -= num;
		}

		numerator   /= num;
		denominator /= denom;
	}
};

template <typename Elem>
std::ostream& operator << (std::ostream& out, const Fraction<Elem>& frac)
{
	return out << frac.numerator << "/" << frac.denominator;
}

template <typename Elem>
Fraction<Elem> operator- (const Elem& elem, const Fraction<Elem>& frac)
{
	return 	Fraction<Elem> {elem} - frac;
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

	void operator -= (const Row<Elem>& other)
	{
		for (size_t i = 0; i < r_size; i++)
			pnt[i] = pnt[i] - other.pnt[i];
	}

	Row<Elem> operator* (const Elem& elem)
	{
		Row<Elem> row{size()};

		for (size_t i = 0; i < size(); i++)
			row[i] = pnt[i] * elem;
	
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

	void Gauss()
	{
		size_t n_zero{0};
		
		for (size_t i = 0; i < size(); i++)
		{
			if (elem[i][i] == Elem{0})
			{
				for (n_zero = i + 1; n_zero < size(); n_zero++)
					if (!(elem[n_zero][i] == Elem{0}))
					{
						std::swap(elem[i], elem[n_zero]);
						break;
					}

				if (n_zero == size()) return;
			}

			for (size_t j = i + 1; j < size(); j++)
				elem[j] -= elem[i] * (elem[j][i] / elem[i][i]);
		}
	}

	Elem Determinant() const
	{
		Matrix<Fraction<Elem>> matrix{size()};

		for (size_t i = 0; i < size(); i++)
		for (size_t j = 0; j < size(); j++)
			matrix[i][j].numerator = elem[i][j];

		matrix.Gauss();
		matrix.Print();

		Fraction<Elem> determinant{1};

		for (size_t i = 0; i < matrix.size(); i++)
			determinant = determinant * matrix[i][i];

		if (determinant.denominator == Elem{0})
			return Elem{0};

		return determinant.numerator / determinant.denominator;
	}

	void Print() const
	{
		for (size_t i = 0; i < elem.size(); i++)
		{
			for (size_t j = 0; j < elem[i].size(); j++)
				std::cout << elem[i][j] << " ";

			std::cout << std::endl;
		}
	}

	size_t size() const { return elem.size(); }

	Row<Elem>& operator[] (size_t it) const { return elem[it]; }
};

int main()
{
	Matrix<int> m{4};
	m.Fill();

	std::cout << m.Determinant() << std::endl;

	return 0;
}