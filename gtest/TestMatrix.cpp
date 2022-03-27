#include <gtest/gtest.h>
#include "../matrix.hpp"

class TestLazyMatrix: public ::testing::Test
{
protected:
    static const size_t size = 5;
    matrix::LazyMatrix<int> m{size};

	void SetUp()
	{
        for (size_t i = 0; i < m.size(); ++i)
        for (size_t j = 0; j < m.size(); ++j)
            m[i][j] = std::rand();
	}

	void TearDown()	{}
};

using eqMatrix = matrix::LazyMatrix<int>::equivalence;

TEST_F (TestLazyMatrix, same_matrix)
{
	EXPECT_EQ(m == m, eqMatrix::equal_buf);
}

TEST_F (TestLazyMatrix, equal_buf)
{
    matrix::LazyMatrix<int> a{m};

	EXPECT_EQ(a == m, eqMatrix::equal_buf);
}

TEST_F (TestLazyMatrix, equal_elems)
{
    matrix::LazyMatrix<int> a{m};
    a *= 1;

	EXPECT_EQ(a == m, eqMatrix::equal_elems);
}

TEST_F (TestLazyMatrix, not_equal)
{
    matrix::LazyMatrix<int> a{m};
    a += matrix::LazyMatrix<int> (m.size(), 1);

	EXPECT_EQ(a == m, eqMatrix::not_equal);
}

TEST_F (TestLazyMatrix, op_equal)
{
    EXPECT_EQ (m == m, eqMatrix::equal_buf);
}

TEST_F (TestLazyMatrix, copy_ctor)
{
    matrix::LazyMatrix<int> a{m};

	EXPECT_EQ(a == m, eqMatrix::equal_buf);
}

TEST_F (TestLazyMatrix, copy_assign)
{
    matrix::LazyMatrix<int> a{};

    a = m;

    EXPECT_EQ (m == a, eqMatrix::equal_buf);
}

TEST_F (TestLazyMatrix, move_ctor)
{
    matrix::LazyMatrix<int> b{m};
    matrix::LazyMatrix<int> a{std::move (m)};

    EXPECT_EQ(a == b, eqMatrix::equal_buf);
}

TEST_F (TestLazyMatrix, move_assign)
{
    matrix::LazyMatrix<int> a{m};
    matrix::LazyMatrix<int> b{};
    b = std::move (m);

    EXPECT_EQ(a == b, eqMatrix::equal_buf);
}

class TestRow: public ::testing::Test
{
protected:
    static const size_t size = 5;
    matrix::Row<int> m{size};

	void SetUp()
	{
        for (size_t i = 0; i < m.size(); ++i)
            m[i] = std::rand();
	}

	void TearDown()	{}
};

TEST_F (TestRow, same_row)
{
	EXPECT_EQ(m.is_same(m), true);
}

TEST_F (TestRow, copy_ctor)
{
    matrix::Row<int> a{m};

	EXPECT_EQ(m.is_same(a), true);
}

TEST_F (TestRow, copy_assign)
{
    matrix::Row<int> a{};
    a = m;

    EXPECT_EQ (m.is_same(a), true);
}

TEST_F (TestRow, move_ctor)
{
    matrix::Row<int> b{m};
    matrix::Row<int> a{std::move (m)};

    EXPECT_EQ(a.is_same(b), true);
}

TEST_F (TestRow, move_assign)
{
    matrix::Row<int> a{m};
    matrix::Row<int> b{};
    b = std::move (m);

    EXPECT_EQ(a.is_same(b), true);
}


struct ThrowElem
{
    int counter;

    ThrowElem(int cnt = 0): counter{cnt} {}
    ThrowElem(const ThrowElem&) = default;
    ThrowElem(ThrowElem&&)      = default;
   ~ThrowElem()                 = default;

    ThrowElem& operator= (const ThrowElem& other)
    {
        if (this != &other)
        {
            if (other.counter % 2 != 0)
                throw 1;

            counter = other.counter;
        }
        return *this;
    }

    ThrowElem& operator= (const ThrowElem&& other) noexcept
    {
        if (this != &other)
            counter = other.counter;

        return *this;
    }
};

TEST (ThrowElem, throwcopy)
{
    matrix::Row<ThrowElem> a(5), b(5);
    b[1] = ThrowElem(1);
    
    try {
        a = b;
    } catch (int) {
        std::cout << "caught exception" << std::endl;
    }
}
