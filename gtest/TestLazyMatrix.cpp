#include <gtest/gtest.h>
#include "../matrix.hpp"

class TestLazyMatrix : public ::testing::Test
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