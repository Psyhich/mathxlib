#include <cstddef>
#include <stdexcept>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fmt/color.h>

#include "unittest_common.h"
#include "matrixes/matrix.h"
#include "matrixes/operations.h"

using namespace MxLib;

TEST(MatrixRandomizationTest, RandomizeRightRangeTestSuccessful_1)
{
	MatrixD toRandomize{5};
	Randomize(toRandomize, 0, 10);

	EXPECT_THAT(toRandomize, IsMatrixValuesInRange(0, 10));
}
TEST(MatrixRandomizationTest, RandomizeRightRangeTestSuccessful_2)
{
	MatrixD toRandomize{5};
	Randomize(toRandomize, -10, 10);

	EXPECT_THAT(toRandomize, IsMatrixValuesInRange(-10, 10));
}
TEST(MatrixRandomizationTest, RandomizeRightRangeTestSuccessful_3)
{
	MatrixD toRandomize{5};
	Randomize(toRandomize, 10, 11);

	EXPECT_THAT(toRandomize, IsMatrixValuesInRange(10, 11));
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

	const Matrix result{Multiply(leftMatrix, rightMatrix)};
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

	EXPECT_THROW({(void)Multiply(leftMatrix, rightMatrix);}, std::length_error);
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
	Matrix<int> values;
	SetAll(values, 0);

	const Matrix<int> expected;
	EXPECT_THAT(values, IsEqualMatrix(expected));
}
TEST(MatrixSetAllTest, ChainedSetAllValuesOfMatrixTestSuccessful)
{
	Matrix values{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	
	SetAll(SetAll(SetAll(values, 0), 20), 25);

	const Matrix expected{
		{ 25, 25, 25 },
		{ 25, 25, 25 },
		{ 25, 25, 25 }
	};
	EXPECT_THAT(values, IsEqualMatrix(expected));
}

TEST(MatrixDotProductTest, MatrixDotProductTestSuccessful)
{
	const Matrix lMatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	const Matrix rMatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};

	const Matrix result = lMatrix * rMatrix;
	const Matrix expected{
		{ 30, 36, 42 },
		{ 66, 81, 96 },
		{ 102, 126, 150 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixDotProductTest, DotProductOfDifferentMatrixesTestSuccessful)
{
	const Matrix lMatrix{
		{ 1, 3 },
		{ 4, 6 },
		{ 7, 9 },
		{ 7, 9 },
		{ 7, 9 },
		{ 7, 9 }
	};
	const Matrix rMatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
	};

	const Matrix result = lMatrix * rMatrix;
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
TEST(MatrixDotProductTest, DotProductOfDifferentMatrixesWithIncompatibleDimensionsTestUnsuccessful_1)
{
	const Matrix lMatrix{
		{ 7, 9 },
		{ 7, 9 }
	};
	const Matrix rMatrix{
		{ 1, 2, 3 },
	};

	EXPECT_THROW({(void)(lMatrix * rMatrix);}, std::length_error);
}
TEST(MatrixDotProductTest, DotProductOfDifferentMatrixesWithIncompatibleDimensionsTestUnsuccessful_2)
{
	const Matrix lMatrix{
		{ 7 },
		{ 7 }
	};
	const Matrix rMatrix{
		{ 1, 2, 3 },
		{ 1, 2, 3 }
	};

	EXPECT_THROW({(void)(lMatrix * rMatrix);}, std::length_error);
}

TEST(MatrixAdditionTest, MatrixAdditionTestSuccessful_1)
{
	const Matrix lMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};
	const Matrix rMatrix{
		{ 5, 6 },
		{ 7, 8 }
	};

	const Matrix result = lMatrix + rMatrix;
	const Matrix expected{
		{ 6, 8 },
		{ 10, 12 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, EmptyMatrixAdditionTestSuccessful)
{
	const Matrix lMatrix{ { 0 } };
	const Matrix rMatrix{ { 0 } };

	const Matrix result = lMatrix + rMatrix;
	const Matrix<int> expected{ { 0 } };
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, MatrixSelfAdditionTestSuccessful)
{
	const Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};

	const Matrix result = lMatrix + lMatrix;
	const Matrix expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, EmptyMatrixSelfAdditionTestSuccessful)
{
	const Matrix<int> lMatrix;
	const Matrix<int> result = lMatrix + lMatrix;
	const Matrix<int> expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, DifferentMatrixAdditionTestUnsuccessful)
{
	const Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rMatrix{
		{ 1, 6, 0 },
		{ 8, 9, 1 },
		{ 10, 11, 20 }
	};

	EXPECT_THROW({ (void)(lMatrix + rMatrix); }, std::length_error);
}
TEST(MatrixAdditionTest, MatrixAssignmentAdditionTestSuccessful)
{
	Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rMatrix{
		{ 1, 6 },
		{ 8, 9 },
	};

	lMatrix += rMatrix;
	const Matrix expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, MatrixSelfAssignmentAdditionTestSuccessful)
{
	Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};

	lMatrix += lMatrix;
	const Matrix expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, EmptyMatrixAssignmentAdditionTestSuccessful)
{
	Matrix<int> lMatrix;
	const Matrix<int> rMatrix;

	lMatrix += rMatrix;
	const Matrix<int> expected;
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, EmptyMatrixSelfAssignmentAdditionTestSuccessful)
{
	Matrix<int> lMatrix;
	lMatrix += lMatrix;
	const Matrix<int> expected;
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(MatrixAdditionTest, DifferentMatrixAssignmentAdditionTestUnsuccessful)
{
	Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};

	const Matrix rMatrix{
		{ 1, 6, 10, 12, 20, 40, 0 },
		{ 8, 9, 10, 12, 20, 40, 0 }
	};

	EXPECT_THROW({(void)(lMatrix += rMatrix);}, std::length_error);
}

TEST(MatrixSubtractionTest, MatrixSubtractionTestSuccessful)
{
	const Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rMatrix{
		{ 2, 8 },
		{ 6, 7 }
	};
	const Matrix result = lMatrix - rMatrix;
	const Matrix expected{
		{ -1, -2 },
		{ 2, 2 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixSubtractionTest, MatrixSelfSubtractionTestSuccessful)
{
	const Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix result = lMatrix - lMatrix;
	const Matrix expected{
		{ 0, 0 },
		{ 0, 0 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixSubtractionTest, EmptyMatrixSelfSubtractionTestSuccessful)
{
	const Matrix<int> lMatrix;
	const Matrix<int> result = lMatrix - lMatrix;
	const Matrix<int> expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixSubtractionTest, DifferentMatrixesSubtractionTestUnsuccessful)
{
	const Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rMatrix{
		{ 0, 0, 1 },
		{ 0, 0, 1 }
	};
	EXPECT_THROW({(void)(lMatrix - rMatrix);}, std::length_error);
}
TEST(MatrixSubtractionTest, MatrixAssignmentSubtractionTestSuccessful)
{
	Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rMatrix{
		{ 2, 8 },
		{ 6, 7 }
	};
	lMatrix -= rMatrix;
	const Matrix expected{
		{ -1, -2 },
		{ 2, 2 }
	};
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(MatrixSubtractionTest, MatrixSelfAssignmentSubtractionTestSuccessful)
{
	Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	lMatrix -= lMatrix;
	const Matrix expected{
		{ 0, 0 },
		{ 0, 0 }
	};
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(MatrixSubtractionTest, EmptyMatrixSelfAssignmentSubtractionTestSuccessful)
{
	Matrix<int> lMatrix;
	lMatrix -= lMatrix;
	const Matrix<int> expected;
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(MatrixSubtractionTest, DifferentMatrixesAssignmentSubtractionTestUnsuccessful)
{
	Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix rMatrix{
		{ 0, 0, 1 },
		{ 0, 0, 1 }
	};
	EXPECT_THROW({(void)(lMatrix -= rMatrix);}, std::length_error);
}

TEST(MatrixScalarMultiplication, ScalarMultiplicationTestSuccessful)
{
	const Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix result{lMatrix * 2.0};
	const Matrix expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarMultiplication, EmptyMatrixScalarMultiplicationTestSuccessful)
{
	const Matrix<int> lMatrix;
	const Matrix<int> result{lMatrix * 2};
	const Matrix<int> expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarDivision, MatrixScalarDivisionTestSuccessful)
{
	const Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix result{lMatrix / 2.};
	const Matrix<double> expected{
		{ 0.5, 3 },
		{ 4, 4.5 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarDivision, EmptyMatrixScalarDivisionTestSuccessful)
{
	const Matrix<double> lMatrix;
	const Matrix<double> result{lMatrix / 2};
	const Matrix<double> expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarAddition, MatrixScalarAdditionTestSuccessful)
{
	const Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix result{lMatrix + 2};
	const Matrix expected{
		{ 3, 8 },
		{ 10, 11 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarAddition, EmptyMatrixScalarAdditionTestSuccessful)
{
	const Matrix<int> lMatrix;
	const Matrix<int> result{lMatrix + 2};
	const Matrix<int> expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarSubtraction, MatrixScalarSubtractionTestSuccessful)
{
	const Matrix lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const Matrix result{lMatrix - 2};
	const Matrix expected{
		{ -1, 4 },
		{ 6, 7 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixScalarSubtraction, EmptyMatrixScalarSubtractionTestSuccessful)
{
	const Matrix<int> lMatrix;
	const Matrix<int> result{lMatrix - 2};
	const Matrix<int> expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}

TEST(MatrixNegative, MatrixNegativeTestSuccessful)
{
	const Matrix lMatrix{
		{ 1, -10 },
		{ -7, 15 }
	};
	const Matrix result{-lMatrix};
	const Matrix expected{
		{ -1, 10 },
		{ 7, -15 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixNegative, EmptyMatrixNegativeTestSuccessful)
{
	const Matrix<int> lMatrix;
	const Matrix<int> result{-lMatrix};
	const Matrix<int> expected;
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixNegative, MatrixChainedNegativeTestSuccessful)
{
	const Matrix lMatrix{
		{ 1, -10 },
		{ -7, 15 }
	};
	const Matrix result{- - -lMatrix};
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
