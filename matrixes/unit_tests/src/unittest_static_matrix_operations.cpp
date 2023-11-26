#include "unittest_common.h"

#include "matrixes/matrix.h"
#include "matrixes/operations.h"
#include "matrixes/algorithms.h"


TEST(StaticMatrixRandomizationTest, RandomizeRightRangeTestSuccessful_1)
{
	MxLib::Matrix3d toRandomize;
	Randomize(toRandomize, 0, 10);

	EXPECT_THAT(toRandomize, IsMatrixValuesInRange(0, 10));
}
TEST(StaticMatrixRandomizationTest, RandomizeRightRangeTestSuccessful_2)
{
	MxLib::SMatrix<double, 5, 5> toRandomize;
	Randomize(toRandomize, -10, 10);

	EXPECT_THAT(toRandomize, IsMatrixValuesInRange(-10, 10));
}
TEST(StaticMatrixRandomizationTest, RandomizeRightRangeTestSuccessful_3)
{
	MxLib::SMatrix<double, 5, 5> toRandomize;
	Randomize(toRandomize, 10, 11);

	EXPECT_THAT(toRandomize, IsMatrixValuesInRange(10, 11));
}

TEST(StaticMatrixMultiplyTest, SimpleMatrixMultiplicationTestSuccessful)
{
	MxLib::Matrix2i leftMatrix
	{
		{ 1, 2 },
		{ 4, 5 }
	};
	const MxLib::Matrix2i rightMatrix
	{
		{ 1, 2 },
		{ 4, 5 }
	};
	const MxLib::Matrix2i expected
	{
		{ 1, 4 },
		{ 16, 25 }
	};

	const MxLib::Matrix2i result{Multiply(leftMatrix, rightMatrix)};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixMultiplyTest, MatrixSelfMultiplicationTestSuccessful)
{
	const MxLib::SMatrix<int, 2, 3> leftMatrix
	{
		{ 1, 2, 3 },
		{ 4, 5, 6 }
	};
	const MxLib::SMatrix<int, 2, 3> expected
	{
		{ 1, 4, 9 },
		{ 16, 25, 36 }
	};

	const MxLib::SMatrix<int, 2, 3> result{Multiply(leftMatrix, leftMatrix)};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}

TEST(StaticMatrixSetAllTest, SetAllValuesOfMatrixTestSuccessful)
{
	MxLib::Matrix3i values{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	SetAll(values, 0);
	const MxLib::Matrix3i expected{
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 }
	};
	EXPECT_THAT(values, IsEqualMatrix(expected));
}
TEST(StaticMatrixSetAllTest, SetAllValuesOfEmptyMatrixTestSuccessful)
{
	MxLib::Matrix3i values;
	SetAll(values, 0);

	const MxLib::Matrix3i expected{};
	EXPECT_THAT(values, IsEqualMatrix(expected));
}
TEST(StaticMatrixSetAllTest, ChainedSetAllValuesOfMatrixTestSuccessful)
{
	MxLib::Matrix3i values{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	
	SetAll(SetAll(SetAll(values, 0), 20), 25);

	const MxLib::Matrix3i expected{
		{ 25, 25, 25 },
		{ 25, 25, 25 },
		{ 25, 25, 25 }
	};
	EXPECT_THAT(values, IsEqualMatrix(expected));
}

TEST(StaticMatrixDotProductTest, MatrixDotProductTestSuccessful)
{
	const MxLib::Matrix3i lMatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	const MxLib::Matrix3i rMatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};

	const MxLib::Matrix3i result{lMatrix * rMatrix};
	const MxLib::Matrix3i expected{
		{ 30, 36, 42 },
		{ 66, 81, 96 },
		{ 102, 126, 150 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixDotProductTest, DotProductOfDifferentMatrixesTestSuccessful)
{
	const MxLib::SMatrix<int, 6, 2> lMatrix{
		{ 1, 3 },
		{ 4, 6 },
		{ 7, 9 },
		{ 7, 9 },
		{ 7, 9 },
		{ 7, 9 }
	};
	const MxLib::SMatrix<int, 2, 3> rMatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
	};

	const MxLib::SMatrix result{lMatrix * rMatrix};
	const MxLib::SMatrix<int, 6, 3> expected{
		{ 13, 17, 21 },
		{ 28, 38, 48 },
		{ 43, 59, 75 },
		{ 43, 59, 75 },
		{ 43, 59, 75 },
		{ 43, 59, 75 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixDotProductTest, DotProductOfStaticAndDynamicMatrixesTestSuccessful)
{
	const MxLib::SMatrix<int, 6, 2> lMatrix{
		{ 1, 3 },
		{ 4, 6 },
		{ 7, 9 },
		{ 7, 9 },
		{ 7, 9 },
		{ 7, 9 }
	};
	const MxLib::Matrix<int> rMatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
	};

	const MxLib::Matrix result{lMatrix * rMatrix};
	const MxLib::SMatrix<int, 6, 3> expected{
		{ 13, 17, 21 },
		{ 28, 38, 48 },
		{ 43, 59, 75 },
		{ 43, 59, 75 },
		{ 43, 59, 75 },
		{ 43, 59, 75 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}

TEST(StaticMatrixAdditionTest, MatrixAdditionTestSuccessful_1)
{
	const MxLib::Matrix2i lMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};
	const MxLib::Matrix2i rMatrix{
		{ 5, 6 },
		{ 7, 8 }
	};

	const MxLib::SMatrix result = lMatrix + rMatrix;
	const MxLib::Matrix2i expected{
		{ 6, 8 },
		{ 10, 12 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixAdditionTest, EmptyMatrixAdditionTestSuccessful)
{
	const MxLib::SMatrix<int, 1, 1> lMatrix{ { 0 } };
	const MxLib::SMatrix<int, 1, 1> rMatrix{ { 0 } };

	const MxLib::SMatrix result = lMatrix + rMatrix;
	const MxLib::SMatrix<int, 1, 1> expected{ { 0 } };
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixAdditionTest, MatrixSelfAdditionTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};

	const MxLib::SMatrix result = lMatrix + lMatrix;
	const MxLib::Matrix2i expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixAdditionTest, EmptyMatrixSelfAdditionTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{};
	const MxLib::Matrix2i result = lMatrix + lMatrix;
	const MxLib::Matrix2i expected{};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixAdditionTest, MatrixAssignmentAdditionTestSuccessful)
{
	MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const MxLib::Matrix2i rMatrix{
		{ 1, 6 },
		{ 8, 9 },
	};

	lMatrix += rMatrix;
	const MxLib::Matrix2i expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(StaticMatrixAdditionTest, MatrixSelfAssignmentAdditionTestSuccessful)
{
	MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};

	lMatrix += lMatrix;
	const MxLib::Matrix2i expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(StaticMatrixAdditionTest, EmptyMatrixAssignmentAdditionTestSuccessful)
{
	MxLib::SMatrix<int, 2, 2> lMatrix{};
	const MxLib::SMatrix<int, 2, 2> rMatrix{};

	lMatrix += rMatrix;
	const MxLib::SMatrix<int, 2, 2> expected{};
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(StaticMatrixAdditionTest, EmptyMatrixSelfAssignmentAdditionTestSuccessful)
{
	MxLib::SMatrix<int, 2, 2> lMatrix{};
	lMatrix += lMatrix;
	const MxLib::SMatrix<int, 2, 2> expected{};
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(StaticMatrixAdditionTest, DifferentMatrixAssignmentAdditionTestUnsuccessful)
{
	MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};

	const MxLib::SMatrix<int, 2, 7> rMatrix{
		{ 1, 6, 10, 12, 20, 40, 0 },
		{ 8, 9, 10, 12, 20, 40, 0 }
	};

	EXPECT_THROW({(void)(lMatrix += rMatrix);}, std::length_error);
}

TEST(StaticMatrixSubtractionTest, MatrixSubtractionTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const MxLib::Matrix2i rMatrix{
		{ 2, 8 },
		{ 6, 7 }
	};
	const MxLib::SMatrix result = lMatrix - rMatrix;
	const MxLib::Matrix2i expected{
		{ -1, -2 },
		{ 2, 2 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixSubtractionTest, MatrixSelfSubtractionTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const MxLib::SMatrix result = lMatrix - lMatrix;
	const MxLib::Matrix2i expected{
		{ 0, 0 },
		{ 0, 0 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixSubtractionTest, EmptyMatrixSelfSubtractionTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{};
	const MxLib::Matrix2i result = lMatrix - lMatrix;
	const MxLib::Matrix2i expected{};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixSubtractionTest, MatrixAssignmentSubtractionTestSuccessful)
{
	MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const MxLib::Matrix2i rMatrix{
		{ 2, 8 },
		{ 6, 7 }
	};
	lMatrix -= rMatrix;
	const MxLib::Matrix2i expected{
		{ -1, -2 },
		{ 2, 2 }
	};
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(StaticMatrixSubtractionTest, MatrixSelfAssignmentSubtractionTestSuccessful)
{
	MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	lMatrix -= lMatrix;
	const MxLib::Matrix2i expected{
		{ 0, 0 },
		{ 0, 0 }
	};
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(StaticMatrixSubtractionTest, EmptyMatrixSelfAssignmentSubtractionTestSuccessful)
{
	MxLib::Matrix2i lMatrix;
	lMatrix -= lMatrix;
	const MxLib::Matrix2i expected{};
	EXPECT_THAT(lMatrix, IsEqualMatrix(expected));
}
TEST(StaticMatrixSubtractionTest, DifferentMatrixesAssignmentSubtractionTestUnsuccessful)
{
	MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const MxLib::SMatrix<int, 2, 3> rMatrix{
		{ 0, 0, 1 },
		{ 0, 0, 1 }
	};
	EXPECT_THROW({(void)(lMatrix -= rMatrix);}, std::length_error);
}

TEST(StaticMatrixScalarMultiplication, ScalarMultiplicationTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const MxLib::SMatrix result{lMatrix * 2};
	const MxLib::Matrix2i expected{
		{ 2, 12 },
		{ 16, 18 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixScalarMultiplication, EmptyMatrixScalarMultiplicationTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{};
	const MxLib::Matrix2i result{lMatrix * 2};
	const MxLib::Matrix2i expected{};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixScalarDivision, MatrixScalarDivisionTestSuccessful)
{
	const MxLib::Matrix2d lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const MxLib::SMatrix result{lMatrix / 2.};
	const MxLib::Matrix2d expected{
		{ 0.5, 3 },
		{ 4, 4.5 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixScalarDivision, EmptyMatrixScalarDivisionTestSuccessful)
{
	const MxLib::Matrix2d lMatrix{};
	const MxLib::Matrix2d result{lMatrix / 2};
	const MxLib::Matrix2d expected{};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixScalarAddition, MatrixScalarAdditionTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const MxLib::SMatrix result{lMatrix + 2};
	const MxLib::Matrix2i expected{
		{ 3, 8 },
		{ 10, 11 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixScalarAddition, EmptyMatrixScalarAdditionTestSuccessful)
{
	const MxLib::SMatrix<int, 0, 0> lMatrix{};
	const MxLib::SMatrix<int, 0, 0> result{lMatrix + 2};
	const MxLib::SMatrix<int, 0, 0> expected{};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixScalarSubtraction, MatrixScalarSubtractionTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{
		{ 1, 6 },
		{ 8, 9 }
	};
	const MxLib::SMatrix result{lMatrix - 2};
	const MxLib::Matrix2i expected{
		{ -1, 4 },
		{ 6, 7 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixScalarSubtraction, EmptyMatrixScalarSubtractionTestSuccessful)
{
	const MxLib::SMatrix<int, 0, 0> lMatrix{};
	const MxLib::SMatrix<int, 0, 0> result{lMatrix - 2};
	const MxLib::SMatrix<int, 0, 0> expected{};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}

TEST(StaticMatrixNegative, MatrixNegativeTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{
		{ 1, -10 },
		{ -7, 15 }
	};
	const MxLib::SMatrix result{-lMatrix};
	const MxLib::Matrix2i expected{
		{ -1, 10 },
		{ 7, -15 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixNegative, EmptyMatrixNegativeTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{};
	const MxLib::Matrix2i result{-lMatrix};
	const MxLib::Matrix2i expected{};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(StaticMatrixNegative, MatrixChainedNegativeTestSuccessful)
{
	const MxLib::Matrix2i lMatrix{
		{ 1, -10 },
		{ -7, 15 }
	};
	const MxLib::SMatrix result{- - -lMatrix};
	const MxLib::Matrix2i expected{
		{ -1, 10 },
		{ 7, -15 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}

TEST(StaticMatrixEqualityCheckerTest, EqualMatrixEqualityCheckerTest)
{
	const MxLib::Matrix2i matrix1{
		{ 1, 10 },
		{ 6, 0 }
	};
	const MxLib::Matrix2i matrix2{
		{ 1, 10 },
		{ 6, 0 }
	};
	EXPECT_TRUE(IsEqualTo(matrix1, matrix2));
}
TEST(StaticMatrixEqualityCheckerTest, NonEqualMatrixEqualityCheckerTest)
{
	const MxLib::Matrix2i matrix1{
		{ 1, 10 },
		{ 6, 0 }
	};
	const MxLib::Matrix2i matrix2{
		{ 0, 11 },
		{ 6, 1 }
	};
	EXPECT_FALSE(IsEqualTo(matrix1, matrix2));
}
TEST(StaticMatrixEqualityCheckerTest, DifferentDimensionsMatrixEqualityCheckerTest_1)
{
	const MxLib::SMatrix<int, 2, 1> matrix1{
		{ 1 },
		{ 6 }
	};
	const MxLib::SMatrix<int, 1, 2> matrix2{
		{ 0, 11 }
	};
	EXPECT_FALSE(IsEqualTo(matrix1, matrix2));
}
TEST(StaticMatrixEqualityCheckerTest, DifferentDimensionsMatrixEqualityCheckerTest_2)
{
	const MxLib::Matrix2i matrix1{
		{ 1, 10 },
		{ 6, 10 }
	};
	const MxLib::SMatrix<int, 1, 2> matrix2{
		{ 0, 11 }
	};
	EXPECT_FALSE(IsEqualTo(matrix1, matrix2));
}
TEST(StaticMatrixTransposeTest, StaticMatrixTranspositionTest_1)
{
	const MxLib::SMatrix<int, 2, 1> matrix1{
		{ 1 },
		{ 10 }
	};
	const MxLib::SMatrix<int, 1, 2> matrix2{MxLib::algo::Transpose(matrix1)};
}
