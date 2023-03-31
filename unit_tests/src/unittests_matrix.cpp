#include "unittest_common.h"
#include "matrix_algorithms.h"
#include <stdexcept>

using namespace MxLib;
using namespace MxLib::algo;


class MatrixConstructionTest :
	public ::testing::Test
{
};

TEST_F(MatrixConstructionTest, ColsRowsConstructionTestSuccessful_1)
{
	const Matrix newMatrix = Matrix{2, 3};
	ASSERT_EQ(newMatrix.Rows(), 2);
	ASSERT_EQ(newMatrix.Cols(), 3);
}
TEST_F(MatrixConstructionTest, ColsRowsConstructionTestSuccessful_2)
{
	const Matrix newMatrix = Matrix{0, 0};
	ASSERT_EQ(newMatrix.Rows(), 0);
	ASSERT_EQ(newMatrix.Cols(), 0);
}
TEST_F(MatrixConstructionTest, ColsRowsConstructionTestSuccessful_3)
{
	const Matrix newMatrix = Matrix{0, 5};
	ASSERT_EQ(newMatrix.Rows(), 0);
	ASSERT_EQ(newMatrix.Cols(), 5);
}
TEST_F(MatrixConstructionTest, SquareMatrixConstructionTestSuccessful_1)
{
	const Matrix newMatrix = Matrix{5};
	ASSERT_EQ(newMatrix.Rows(), 5);
	ASSERT_EQ(newMatrix.Cols(), 5);
}
TEST_F(MatrixConstructionTest, SquareMatrixConstructionTestSuccessful_2)
{
	const Matrix newMatrix = Matrix{0};
	ASSERT_EQ(newMatrix.Rows(), 0);
	ASSERT_EQ(newMatrix.Cols(), 0);
}
TEST_F(MatrixConstructionTest, SquareMatrixConstructionTestSuccessful_3)
{
	const Matrix newMatrix = Matrix{10000};
	ASSERT_EQ(newMatrix.Rows(), 10000);
	ASSERT_EQ(newMatrix.Cols(), 10000);
}
TEST_F(MatrixConstructionTest, InitializerListConstructionTestSuccessful_1)
{
	const Matrix newMatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	ASSERT_EQ(newMatrix.Rows(), 3);
	ASSERT_EQ(newMatrix.Cols(), 3);
}
TEST_F(MatrixConstructionTest, InitializerListConstructionTestSuccessful_2)
{
	const Matrix newMatrix{
		{ 1, 2, 3, 2 },
		{ 4, 5, 6, 2 },
		{ 7, 8, 9, 2 }
	};
	ASSERT_EQ(newMatrix.Rows(), 3);
	ASSERT_EQ(newMatrix.Cols(), 4);
}
TEST_F(MatrixConstructionTest, InitializerListConstructionTestSuccessful_3)
{
	const Matrix newMatrix{{}};
	ASSERT_EQ(newMatrix.Rows(), 0);
	ASSERT_EQ(newMatrix.Cols(), 0);
}
TEST_F(MatrixConstructionTest, ByRankConstructionTestSuccessful)
{
	const Matrix newMatrix{5};
	ASSERT_EQ(newMatrix.Rows(), 5);
	ASSERT_EQ(newMatrix.Cols(), 5);
}
TEST_F(MatrixConstructionTest, CopyConstructionTestSuccessful_1)
{
	const Matrix newMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};
	const Matrix copy{newMatrix};

	EXPECT_THAT(copy, IsEqualMatrix(newMatrix));
}
TEST_F(MatrixConstructionTest, CopyConstructionTestSuccessful_2)
{
	const Matrix newMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};
	Matrix copy{{1}};
	copy = newMatrix;

	EXPECT_THAT(copy, IsEqualMatrix(newMatrix));
}
TEST_F(MatrixConstructionTest, MoveConstructorTestSuccessful_1)
{
	Matrix original{
		{ 1, 2 },
		{ 3, 4 }
	};

	const Matrix moved{std::move(original)};

	ASSERT_EQ(original.Rows(), 0); // cppcheck-suppress[accessMoved]
	ASSERT_EQ(original.Cols(), 0); // cppcheck-suppress[accessMoved]

	ASSERT_EQ(moved.Rows(), 2);
	ASSERT_EQ(moved.Cols(), 2);
	
	const Matrix expected{
		{ 1, 2 },
		{ 3, 4 }
	};
	EXPECT_THAT(moved, IsEqualMatrix(expected));
}
TEST_F(MatrixConstructionTest, MoveConstructorTestSuccessful_2)
{
	Matrix original{
		{ 1, 2 },
		{ 3, 4 }
	};

	Matrix moved{{ 1, 2 }};
	moved = std::move(original);

	ASSERT_EQ(original.Rows(), 0); // cppcheck-suppress[accessMoved]
	ASSERT_EQ(original.Cols(), 0); // cppcheck-suppress[accessMoved]

	ASSERT_EQ(moved.Rows(), 2);
	ASSERT_EQ(moved.Cols(), 2);
	
	const Matrix expected{
		{ 1, 2 },
		{ 3, 4 }
	};
	EXPECT_THAT(moved, IsEqualMatrix(expected));
}
TEST(MatrixBoundCheckTest, MatrixInBoundGetterTestSuccessful)
{
	const Matrix testMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};

	EXPECT_THAT(testMatrix(0, 1), testing::DoubleEq(2));
}
TEST(MatrixBoundCheckTest, MatrixOutOfBoundGetterTestUnsuccessful)
{
	const Matrix testMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};

	EXPECT_THROW({ testMatrix(0, 2); }, std::out_of_range);
}
