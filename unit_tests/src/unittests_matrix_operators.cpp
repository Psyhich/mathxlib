#include <cstddef>
#include <stdexcept>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fmt/color.h>

#include "unittest_common.h"
#include "operations.h"

using namespace MxLib;

MATCHER_P2(IsMatrixValuesInRange, rangeStart, rangeEnd, "")
{
	for(std::size_t row = 0; row < arg.Rows(); row++)
	{
		for(std::size_t col = 0; col < arg.Rows(); col++)
		{
			if(arg(row, col) < rangeStart ||
				arg(row, col) > rangeEnd)
			{
				*result_listener << fmt::format(fg(fmt::color::red), "\nERROR:") <<
					fmt::format("\nMatrix value {} on {{{};{}}}"
							" is not in range [{};{}]\n",
							arg(row, col), row, col,
							rangeStart, rangeEnd);
				return false;
			}
		}
	}
	return true;
}

TEST(MatrixRandomizationTest, RandomizeRightRangeTestSuccessfull_1)
{
	Matrix toRandomise{5};
	Randomise(toRandomise, 0, 10);

	EXPECT_THAT(toRandomise, IsMatrixValuesInRange(0, 10));
}
TEST(MatrixRandomizationTest, RandomizeRightRangeTestSuccessfull_2)
{
	Matrix toRandomise{5};
	Randomise(toRandomise, -10, 10);

	EXPECT_THAT(toRandomise, IsMatrixValuesInRange(-10, 10));
}
TEST(MatrixRandomizationTest, RandomizeRightRangeTestSuccessfull_3)
{
	Matrix toRandomise{5};
	Randomise(toRandomise, 10, 11);

	EXPECT_THAT(toRandomise, IsMatrixValuesInRange(10, 11));
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

	const Matrix result = Multiply(leftMatrix, rightMatrix);
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

	const Matrix result = Multiply(leftMatrix, leftMatrix);
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
	EXPECT_THROW({result = Multiply(leftMatrix, rightMatrix);}, std::length_error);
}

TEST(MatrixSetAllTest, SetAllValuesOfMatrixTestSuccessful)
{
	Matrix values{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	SetAll(values, 0);

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
	SetAll(values, 0);

	const Matrix expected;
	EXPECT_THAT(values, IsEqualMatrix(expected));
}
TEST(MatrixSetAllTest, ChainedSetAllValuesOfMatrixTestSuccessful)
{
	Matrix values{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	
	SetAll(SetAll(SetAll(values, 0), 20), 25.5);

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
	const Matrix lmatrix;
	const Matrix rmatrix;

	const Matrix result = lmatrix + rmatrix;
	const Matrix expected;
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
TEST(MatrixAdditionTest, EmptyMatrixSelfAdditionTestSuccessful)
{
	const Matrix lmatrix;
	const Matrix result = lmatrix + lmatrix;
	const Matrix expected;
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
TEST(MatrixAdditionTest, MatrixAssigmentAdditionTestSuccessful)
{
	Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rmatrix{
		{ 1, 6 },
		{ 8, 9 },
	};

	lmatrix += rmatrix;
	const Matrix expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(lmatrix, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, MatrixSelfAssigmentAdditionTestSuccessful)
{
	Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};

	lmatrix += lmatrix;
	const Matrix expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(lmatrix, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, EmptyMatrixAssigmentAdditionTestSuccessful)
{
	Matrix lmatrix;
	const Matrix rmatrix;

	lmatrix += rmatrix;
	const Matrix expected;
	EXPECT_THAT(lmatrix, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, EmptyMatrixSelfAssigmentAdditionTestSuccessful)
{
	Matrix lmatrix;
	lmatrix += lmatrix;
	const Matrix expected;
	EXPECT_THAT(lmatrix, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, DifferentMatrixAssigmentAdditionTestUnsuccessful)
{
	Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};

	const Matrix rmatrix{
		{ 1, 6, 10, 12, 20, 40, 0 },
		{ 8, 9, 10, 12, 20, 40, 0 }
	};

	EXPECT_THROW({(void)(lmatrix += rmatrix);}, std::length_error);
}

TEST(MatrixSubtractionTest, MatrixSubtractionTestSuccessful)
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
TEST(MatrixSubtractionTest, MatrixSelfSubtractionTestSuccessful)
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
TEST(MatrixSubtractionTest, EmptyMatrixSelfSubtractionTestSuccessful)
{
	const Matrix lmatrix;
	const Matrix result = lmatrix - lmatrix;
	const Matrix expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixSubtractionTest, DifferentMatrixesSubtractionTestUnsuccessful)
{
	const Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rmatrix{
		{ 0, 0, 1 },
		{ 0, 0, 1 }
	};
	EXPECT_THROW({(void)(lmatrix - rmatrix);}, std::length_error);
}
TEST(MatrixSubtractionTest, MatrixAssigmentSubtractionTestSuccessful)
{
	Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rmatrix{
		{ 2, 8 },
		{ 6, 7 }
	};
	lmatrix -= rmatrix;
	const Matrix expected{
		{ -1, -2 },
		{ 2, 2 }
	};
	EXPECT_THAT(lmatrix, IsEqualMatrix(expected));
}
TEST(MatrixSubtractionTest, MatrixSelfAssigmentSubtractionTestSuccessful)
{
	Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	lmatrix -= lmatrix;
	const Matrix expected{
		{ 0, 0 },
		{ 0, 0 }
	};
	EXPECT_THAT(lmatrix, IsEqualMatrix(expected));
}
TEST(MatrixSubtractionTest, EmptyMatrixSelfAssigmentSubtractionTestSuccessful)
{
	Matrix lmatrix;
	lmatrix -= lmatrix;
	const Matrix expected;
	EXPECT_THAT(lmatrix, IsEqualMatrix(expected));
}
TEST(MatrixSubtractionTest, DifferentMatrixesAssigmentSubtractionTestUnsuccessful)
{
	Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rmatrix{
		{ 0, 0, 1 },
		{ 0, 0, 1 }
	};
	EXPECT_THROW({(void)(lmatrix -= rmatrix);}, std::length_error);
}

TEST(MatrixScalarMultiplication, ScalarMultiplicationTestSuccessful)
{
	const Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix result{lmatrix * 2.0};
	const Matrix expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarMultiplication, EmptyMatrixeScalarMultiplicationTestSuccessful)
{
	const Matrix lmatrix;
	const Matrix result{lmatrix * 2};
	const Matrix expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarDivision, MatrixScalarDivisionTestSuccessful)
{
	const Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix result{lmatrix / 2};
	const Matrix expected{
		{ 0.5, 3 },
		{ 4, 4.5 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarDivision, EmptyMatrixScalarDivisionTestSuccessful)
{
	const Matrix lmatrix;
	const Matrix result{lmatrix / 2};
	const Matrix expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarAddition, MatrixScalarAdditionTestSuccessful)
{
	const Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix result{lmatrix + 2};
	const Matrix expected{
		{ 3, 8 },
		{ 10, 11 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarAddition, EmptyMatrixScalarAdditionTestSuccessful)
{
	const Matrix lmatrix;
	const Matrix result{lmatrix + 2};
	const Matrix expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarSubtraction, MatrixScalarSubtractionTestSuccessful)
{
	const Matrix lmatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix result{lmatrix - 2};
	const Matrix expected{
		{ -1, 4 },
		{ 6, 7 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarSubtraction, EmptyMatrixScalarSubtractionTestSuccessful)
{
	const Matrix lmatrix;
	const Matrix result{lmatrix - 2};
	const Matrix expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}

TEST(MatrixNegative, MatrixeNegativeTestSuccessful)
{
	const Matrix lmatrix{
		{ 1, -10 },
		{ -7, 15 }
	};
	const Matrix result{-lmatrix};
	const Matrix expected{
		{ -1, 10 },
		{ 7, -15 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixNegative, EmptyMatrixeNegativeTestSuccessful)
{
	const Matrix lmatrix;
	const Matrix result{-lmatrix};
	const Matrix expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixNegative, MatrixeChainedNegativeTestSuccessful)
{
	const Matrix lmatrix{
		{ 1, -10 },
		{ -7, 15 }
	};
	const Matrix result{- - -lmatrix};
	const Matrix expected{
		{ -1, 10 },
		{ 7, -15 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}

TEST(MatrixEqualityCheckerTest, EqualMatrixEqualityCheckerTest)
{
	const Matrix matrix1{
		{ 1, 10 },
		{ 6, 0 }
	};
	const Matrix matrix2{
		{ 1, 10 },
		{ 6, 0 }
	};
	EXPECT_TRUE(IsEqualTo(matrix1, matrix2));
}
TEST(MatrixEqualityCheckerTest, NonEqualMatrixEqualityCheckerTest)
{
	const Matrix matrix1{
		{ 1, 10 },
		{ 6, 0 }
	};
	const Matrix matrix2{
		{ 0, 11 },
		{ 6, 1 }
	};
	EXPECT_FALSE(IsEqualTo(matrix1, matrix2));
}
TEST(MatrixEqualityCheckerTest, DifferentDimensionsMatrixEqualityCheckerTest_1)
{
	const Matrix matrix1{
		{ 1 },
		{ 6 }
	};
	const Matrix matrix2{
		{ 0, 11 }
	};
	EXPECT_FALSE(IsEqualTo(matrix1, matrix2));
}
TEST(MatrixEqualityCheckerTest, DifferentDimensionsMatrixEqualityCheckerTest_2)
{
	const Matrix matrix1{
		{ 1, 10 },
		{ 6, 10 }
	};
	const Matrix matrix2{
		{ 0, 11 }
	};
	EXPECT_FALSE(IsEqualTo(matrix1, matrix2));
}
