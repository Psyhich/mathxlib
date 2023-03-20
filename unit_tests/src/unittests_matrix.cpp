#include "gmock/gmock.h"
#include <cstddef>
#include <stdexcept>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "matrixes.h"

MATCHER_P(IsEqualMatrix, rmatrix, "")
{
	if(arg.GetRows() != rmatrix.GetRows())
	{
		return false;
	}
	if(arg.GetCols() != rmatrix.GetCols())
	{
		return false;
	}

	for(std::size_t row = 0;
		row < rmatrix.GetRows(); row++)
	{
		for(std::size_t col = 0;
			col < rmatrix.GetCols(); col++)
		{
			if(std::abs(arg(row, col) - rmatrix(row, col)) > 1e-6)
			{
				return false;
			}
		}
	}
	return true;
}

class MatrixConstructionTest :
	public ::testing::Test
{
};

TEST_F(MatrixConstructionTest, ColsRowsConstructionTestSuccessful_1)
{
	const Matrix newMatrix = Matrix{2, 3};
	ASSERT_EQ(newMatrix.GetRows(), 2);
	ASSERT_EQ(newMatrix.GetCols(), 3);
}
TEST_F(MatrixConstructionTest, ColsRowsConstructionTestSuccessful_2)
{
	const Matrix newMatrix = Matrix{0, 0};
	ASSERT_EQ(newMatrix.GetRows(), 0);
	ASSERT_EQ(newMatrix.GetCols(), 0);
}
TEST_F(MatrixConstructionTest, ColsRowsConstructionTestSuccessful_3)
{
	const Matrix newMatrix = Matrix{0, 5};
	ASSERT_EQ(newMatrix.GetRows(), 0);
	ASSERT_EQ(newMatrix.GetCols(), 5);
}
TEST_F(MatrixConstructionTest, SquareMatrixConstructionTestSuccessful_1)
{
	const Matrix newMatrix = Matrix{5};
	ASSERT_EQ(newMatrix.GetRows(), 5);
	ASSERT_EQ(newMatrix.GetCols(), 5);
}
TEST_F(MatrixConstructionTest, SquareMatrixConstructionTestSuccessful_2)
{
	const Matrix newMatrix = Matrix{0};
	ASSERT_EQ(newMatrix.GetRows(), 0);
	ASSERT_EQ(newMatrix.GetCols(), 0);
}
TEST_F(MatrixConstructionTest, SquareMatrixConstructionTestSuccessful_3)
{
	const Matrix newMatrix = Matrix{10000};
	ASSERT_EQ(newMatrix.GetRows(), 10000);
	ASSERT_EQ(newMatrix.GetCols(), 10000);
}
TEST_F(MatrixConstructionTest, InitializerListConstructionTestSuccessful_1)
{
	const Matrix newMatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	ASSERT_EQ(newMatrix.GetRows(), 3);
	ASSERT_EQ(newMatrix.GetCols(), 3);
}
TEST_F(MatrixConstructionTest, InitializerListConstructionTestSuccessful_2)
{
	const Matrix newMatrix{
		{ 1, 2, 3, 2 },
		{ 4, 5, 6, 2 },
		{ 7, 8, 9, 2 }
	};
	ASSERT_EQ(newMatrix.GetRows(), 3);
	ASSERT_EQ(newMatrix.GetCols(), 4);
}
TEST_F(MatrixConstructionTest, InitializerListConstructionTestSuccessful_3)
{
	const Matrix newMatrix{{}};
	ASSERT_EQ(newMatrix.GetRows(), 0);
	ASSERT_EQ(newMatrix.GetCols(), 0);
}
TEST_F(MatrixConstructionTest, ByRankConstructionTestSuccessful)
{
	const Matrix newMatrix{5};
	ASSERT_EQ(newMatrix.GetRows(), 5);
	ASSERT_EQ(newMatrix.GetCols(), 5);
}
TEST_F(MatrixConstructionTest, CopyConstructionTestSuccessful_1)
{
	Matrix newMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};
	const Matrix copy{newMatrix};

	EXPECT_THAT(copy, IsEqualMatrix(newMatrix));
}
TEST_F(MatrixConstructionTest, CopyConstructionTestSuccessful_2)
{
	Matrix newMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};
	const Matrix copy = newMatrix;

	EXPECT_THAT(copy, IsEqualMatrix(newMatrix));
}
TEST_F(MatrixConstructionTest, MoveConstructorTestSuccessful_1)
{
	Matrix original{
		{ 1, 2 },
		{ 3, 4 }
	};

	const Matrix moved{std::move(original)};

	ASSERT_EQ(original.GetRows(), 0); // cppcheck-suppress[accessMoved]
	ASSERT_EQ(original.GetCols(), 0); // cppcheck-suppress[accessMoved]

	ASSERT_EQ(moved.GetRows(), 2);
	ASSERT_EQ(moved.GetCols(), 2);
	
	std::size_t num{1};
	for(std::size_t row = 0;
		row < moved.GetRows(); row++)
	{
		for(std::size_t col = 0;
			col < moved.GetCols();
			col++, ++num)
		{
			ASSERT_EQ(moved(row, col), num);
		}
	}
}
TEST_F(MatrixConstructionTest, MoveConstructorTestSuccessful_2)
{
	Matrix original{
		{ 1, 2 },
		{ 3, 4 }
	};

	const Matrix moved = std::move(original);

	ASSERT_EQ(original.GetRows(), 0); // cppcheck-suppress[accessMoved]
	ASSERT_EQ(original.GetCols(), 0); // cppcheck-suppress[accessMoved]

	ASSERT_EQ(moved.GetRows(), 2);
	ASSERT_EQ(moved.GetCols(), 2);
	
	std::size_t num{1};
	for(std::size_t row = 0;
		row < moved.GetRows(); row++)
	{
		for(std::size_t col = 0;
			col < moved.GetCols();
			col++, ++num)
		{
			ASSERT_EQ(moved(row, col), num);
		}
	}
}

TEST(MatrixRandomizationTest, RandomizeRightRangeTestSuccessfull_1)
{
	Matrix toRandomise{5};
	toRandomise.Randomise(0, 10);

	toRandomise.MapInplace(
		[](double val)
		{
			ASSERT_GE(val, 0);
			ASSERT_LE(val, 10);
		});
}
TEST(MatrixRandomizationTest, RandomizeRightRangeTestSuccessfull_2)
{
	Matrix toRandomise{5};
	toRandomise.Randomise(-10, 10);

	toRandomise.MapInplace(
		[](double val)
		{
			ASSERT_GE(val, -10);
			ASSERT_LE(val, 10);
		});
}
TEST(MatrixRandomizationTest, RandomizeRightRangeTestSuccessfull_3)
{
	Matrix toRandomise{5};
	toRandomise.Randomise(10, 11);

	toRandomise.MapInplace(
		[](double val)
		{
			ASSERT_GE(val, 10);
			ASSERT_LE(val, 11);
		});
}

TEST(MatrixTransposeTest, SimpleMatrixTransposeTestSuccessful)
{
	const Matrix toTranspose
	{
		{ 1, 2 },
		{ 3, 4 },
		{ 5, 6 },
		{ 7, 8 },
	};
	const Matrix transposed{toTranspose.Transpose()};
	const Matrix expected
	{
		{ 1, 3, 5, 7 },
		{ 2, 4, 6, 8 }
	};
	
	EXPECT_THAT(transposed, IsEqualMatrix(expected));
}
TEST(MatrixTransposeTest, VectorLikeMatrixTransposeTestSuccessful)
{
	const Matrix toTranspose
	{
		{ 1 },
		{ 3 },
		{ 5 },
		{ 7 },
	};
	const Matrix transposed{toTranspose.Transpose()};
	const Matrix expected
	{
		{ 1, 3, 5, 7 }
	};
	
	EXPECT_THAT(transposed, IsEqualMatrix(expected));
}
TEST(MatrixTransposeTest, RankOneMatrixTransposeTestSuccessful)
{
	const Matrix toTranspose
	{
		{ 1 }
	};
	const Matrix transposed{toTranspose.Transpose()};
	const Matrix expected
	{
		{ 1 }
	};
	
	EXPECT_THAT(transposed, IsEqualMatrix(expected));
}
TEST(MatrixTransposeTest, SeveralMatrixTransposeTestSuccessful)
{
	const Matrix toTranspose
	{
		{ 1, 5, 10, 100 },
		{ 3, 2, 5, 100 },
		{ 5, 6, 2, 100 },
		{ 7, 3, 4, 100 },
	};
	const Matrix expected
	{
		{ 1, 5, 10, 100 },
		{ 3, 2, 5, 100 },
		{ 5, 6, 2, 100 },
		{ 7, 3, 4, 100 },
	};
	const Matrix doubleTransposed{
		toTranspose
			.Transpose()
			.Transpose()
	};
	
	EXPECT_THAT(doubleTransposed, IsEqualMatrix(expected));
}

TEST(MatrixMultiplyTest, SimpleMatrixMultiplicationTestSuccessful)
{
	const Matrix leftMatrix
	{
		{ 1, 2, 3 },
		{ 4, 5, 6 }
	};
	const Matrix rightMatrix
	{
		{ 1, 2, 3 },
		{ 4, 5, 6 }
	};
	const Matrix expected
	{
		{ 1, 4, 9 },
		{ 16, 25, 36 }
	};

	Matrix result;
	EXPECT_NO_THROW({result = leftMatrix.Multiply(rightMatrix);});
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixMultiplyTest, MatrixSelfMultiplicationTestSuccessful)
{
	const Matrix leftMatrix
	{
		{ 1, 2, 3 },
		{ 4, 5, 6 }
	};
	const Matrix expected
	{
		{ 1, 4, 9 },
		{ 16, 25, 36 }
	};

	Matrix result;
	EXPECT_NO_THROW({result = leftMatrix.Multiply(leftMatrix);});
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixMultiplyTest, MatrixWrongDimensionsMultiplicationTestUnsuccessful)
{
	const Matrix leftMatrix
	{
		{ 1, 2, 3 },
		{ 4, 5, 6 }
	};
	const Matrix rightMatrix
	{
		{ 1, 2, 3, 9 },
		{ 4, 5, 6, 10 }
	};

	Matrix result;
	EXPECT_THROW({result = leftMatrix.Multiply(rightMatrix);}, std::length_error);
}
TEST(MatrixDeterminantTest, SquareMatrixDeterminantTestSuccessful_1)
{
	const Matrix toGetDeterminant
	{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	EXPECT_THAT(toGetDeterminant.Determinant(), testing::DoubleEq(0));
}
TEST(MatrixDeterminantTest, SquareMatrixDeterminantTestSuccessful_2)
{
	const Matrix toGetDeterminant
	{
		{ 8, 2.8, 3 },
		{ 4, 6, 6.9 },
		{ 8.1, 9, 10 }
	};
	EXPECT_THAT(toGetDeterminant.Determinant(), testing::DoubleNear(-10.10799999999999, 1e-6));
}
TEST(MatrixDeterminantTest, LargeSquareMatrixDeterminantTestSuccessful)
{
	// 7 - is largest number when calculation takes adequate time(less then second)
	Matrix toGetDeterminant{ 7 };

	long number = 1;
	for(std::size_t row = 0; row < toGetDeterminant.GetRows(); row++)
	{
		for(std::size_t col = 0; col < toGetDeterminant.GetCols(); col++)
		{
			toGetDeterminant.Set(row, col, number++);
		}
	}
	EXPECT_THAT(toGetDeterminant.Determinant(), testing::DoubleEq(0));
}
// Currently unimplemeted feature
TEST(MatrixDeterminantTest, NonSquareMatrixDeterminantTestUnsuccessful)
{
	Matrix toGetDeterminant{ 6, 3 };
	EXPECT_THROW({(void)toGetDeterminant.Determinant();}, std::runtime_error);
}

TEST(MatrixSetAllTest, SetAllValuesOfMatrixTestSuccessful)
{
	Matrix values{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	values.SetAll(0);

	const Matrix expected{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 }
	};
	EXPECT_THAT(values, IsEqualMatrix(expected));
}
TEST(MatrixSetAllTest, SetAllValuesOfEmptyMatrixTestSuccessful)
{
	Matrix values;
	values.SetAll(0);

	const Matrix expected{};
	EXPECT_THAT(values, IsEqualMatrix(expected));
}
TEST(MatrixSetAllTest, ChainedSetAllValuesOfMatrixTestSuccessful)
{
	Matrix values{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	values
		.SetAll(0)
		.SetAll(10)
		.SetAll(20)
		.SetAll(30)
		.SetAll(25.5);

	const Matrix expected{
		{ 25.5, 25.5, 25.5 },
		{ 25.5, 25.5, 25.5 },
		{ 25.5, 25.5, 25.5 }
	};
	EXPECT_THAT(values, IsEqualMatrix(expected));
}

TEST(MatrixDotProductTest, MatrixDotProductTestSuccessful)
{
	const Matrix lmatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	const Matrix rmatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};

	const Matrix result = lmatrix * rmatrix;
	const Matrix expected{
		{ 30, 36, 42 },
		{ 66, 81, 96 },
		{ 102, 126, 150 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixDotProductTest, DotProductOfDifferentMatrixesTestSuccessful)
{
	const Matrix lmatrix{
		{ 1, 3 },
		{ 4, 6 },
		{ 7, 9 },
		{ 7, 9 },
		{ 7, 9 },
		{ 7, 9 }
	};
	const Matrix rmatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
	};

	const Matrix result = lmatrix * rmatrix;
	const Matrix expected{
		{ 13, 17, 21 },
		{ 28, 38, 48 },
		{ 43, 59, 75 },
		{ 43, 59, 75 },
		{ 43, 59, 75 },
		{ 43, 59, 75 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixDotProductTest, DotProductOfDifferentMatrixesWithUncompatibleDimensionsTestUnsuccessful_1)
{
	const Matrix lmatrix{
		{ 7, 9 },
		{ 7, 9 }
	};
	const Matrix rmatrix{
		{ 1, 2, 3 },
	};

	EXPECT_THROW({(void)(lmatrix * rmatrix);}, std::length_error);
}
TEST(MatrixDotProductTest, DotProductOfDifferentMatrixesWithUncompatibleDimensionsTestUnsuccessful_2)
{
	const Matrix lmatrix{
		{ 7 },
		{ 7 }
	};
	const Matrix rmatrix{
		{ 1, 2, 3 },
		{ 1, 2, 3 }
	};

	EXPECT_THROW({(void)(lmatrix * rmatrix);}, std::length_error);
}

TEST(MatrixAdjointTest, SquareMatrixAdjointTestSuccesful)
{
	const Matrix toBeProcessed{
		{ 7, 8, 9 },
		{ 6, 5, 4 },
		{ 3, 2, 1 }
	};

	const Matrix result = toBeProcessed.Adjoint();
	const Matrix expected{
		{ -3, 10, -13 },
		{ 6, -20, 26 },
		{ -3, 10, -13 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixAdjointTest, NonSquareMatrixAdjointTestUnsuccesful) {
	const Matrix toBeProcessed{
		{ 7, 8, 9, 10.6 },
		{ 6, 5, 4, 10.6 },
		{ 3, 2, 1, 10.6 }
	};

	EXPECT_THROW({(void)toBeProcessed.Adjoint();}, std::runtime_error);
}

TEST(MatrixAdditionTest, MatrixAdditionTestSuccessful_1)
{
	const Matrix lmatrix{
		{ 1, 2 },
		{ 3, 4 }
	};
	const Matrix rmatrix{
		{ 5, 6 },
		{ 7, 8 }
	};

	const Matrix result = lmatrix + rmatrix;
	const Matrix expected{
		{ 6, 8 },
		{ 10, 12 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, EmptyMatrixAdditionTestSuccessful)
{
	const Matrix lmatrix{};
	const Matrix rmatrix{};

	const Matrix result = lmatrix + rmatrix;
	const Matrix expected{};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, MatrixSelfAdditionTestSuccessful)
{
	const Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};

	const Matrix result = lmatrix + lmatrix;
	const Matrix expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, DifferentMatrixAdditionTestUnsuccessful)
{
	const Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rmatrix{
		{ 1, 6, 0 },
		{ 8, 9, 1 },
		{ 10, 11, 20 }
	};

	Matrix result;
	EXPECT_THROW({ result = lmatrix + rmatrix; }, std::length_error);
}

TEST(MatrixAdditionTest, MatrixSubtractionTestSuccessful)
{
	const Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rmatrix{
		{ 2, 8 },
		{ 6, 7 }
	};
	const Matrix result = lmatrix - rmatrix;
	const Matrix expected{
		{ -1, -2 },
		{ 2, 2 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, MatrixSelfSubtractionTestSuccessful)
{
	const Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix result = lmatrix - lmatrix;
	const Matrix expected{
		{ 0, 0 },
		{ 0, 0 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
