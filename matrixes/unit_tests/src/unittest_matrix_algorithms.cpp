#include "unittest_common.h"
#include "matrixes/algorithms.h"

using namespace MxLib;
using namespace MxLib::algo;

TEST(MatrixTransposeTest, SimpleMatrixTransposeTestSuccessful)
{
	const Matrix toTranspose
	{
		{ 1, 2 },
		{ 3, 4 },
		{ 5, 6 },
		{ 7, 8 },
	};
	const Matrix transposed{Transpose(toTranspose)};
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
	const Matrix transposed{Transpose(toTranspose)};
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
	const Matrix transposed{Transpose(toTranspose)};
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
		Transpose(Transpose(toTranspose))
	};
	
	EXPECT_THAT(doubleTransposed, IsEqualMatrix(expected));
}

TEST(MatrixDeterminantTest, SquareMatrixDeterminantTestSuccessful_1)
{
	const Matrix toGetDeterminant
	{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	EXPECT_THAT(Determinant(toGetDeterminant), testing::DoubleEq(0));
}
TEST(MatrixDeterminantTest, SquareMatrixDeterminantTestSuccessful_2)
{
	const Matrix toGetDeterminant
	{
		{ 8, 2.8, 3 },
		{ 4, 6, 6.9 },
		{ 8.1, 9, 10 }
	};
	EXPECT_THAT(Determinant(toGetDeterminant), testing::DoubleNear(-10.10799999999999, 1e-6));
}
TEST(MatrixDeterminantTest, LargeSquareMatrixDeterminantTestSuccessful)
{
	// 7 - is largest number when calculation takes adequate time(less then second)
	Matrix toGetDeterminant{ 7 };

	long number = 1;
	for(std::size_t row = 0; row < toGetDeterminant.Rows(); row++)
	{
		for(std::size_t col = 0; col < toGetDeterminant.Cols(); col++)
		{
			toGetDeterminant(row, col) = number++;
		}
	}
	EXPECT_THAT(Determinant(toGetDeterminant), testing::DoubleEq(0));
}
TEST(MatrixDeterminantTest, SingleValueSquareMatrixDeterminantTestSuccessful)
{
	Matrix toGetDeterminant{ { 1 } };
	EXPECT_THAT(Determinant(toGetDeterminant), testing::DoubleEq(1));
}
TEST(MatrixDeterminantTest, NonSquareMatrixDeterminantTestUnsuccessful)
{
	Matrix toGetDeterminant{ 6, 3 };
	EXPECT_THROW({(void)Determinant(toGetDeterminant);}, std::runtime_error);
}

TEST(MatrixAdjointTest, SquareMatrixAdjointTestSuccessful)
{
	const Matrix toBeProcessed{
		{ 7, 8, 9 },
		{ 6, 5, 4 },
		{ 3, 2, 1 }
	};

	const Matrix result = Adjoint(toBeProcessed);
	const Matrix expected{
		{ -3, 10, -13 },
		{ 6, -20, 26 },
		{ -3, 10, -13 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixAdjointTest, NonSquareMatrixAdjointTestUnsuccessful) {
	const Matrix toBeProcessed{
		{ 7, 8, 9, 10.6 },
		{ 6, 5, 4, 10.6 },
		{ 3, 2, 1, 10.6 }
	};

	EXPECT_THROW({(void)Adjoint(toBeProcessed);}, std::runtime_error);
}
TEST(MatrixInverseTest, SquareMatrixInverseTestSuccessful)
{
	const Matrix toBeProcessed{
		{ 7, 8, 9 },
		{ 6, 5, 4 },
		{ 3, 2, 2 }
	};

	const Matrix result = Inverse(toBeProcessed);
	const Matrix expected{
		{ -2/13.0, -2/13.0, 1 },
		{ 0, 1, -2 },
		{ 3/13.0, -10/13.0, 1 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixInverseTest, NonSquareMatrixInverseTestUnsuccessful)
{
	const Matrix toBeProcessed{
		{ 7, 8, 9, 10.6 },
		{ 6, 5, 4, 10.6 },
		{ 3, 2, 1, 10.6 }
	};
	EXPECT_THROW({(void)Inverse(toBeProcessed);}, std::runtime_error);
}
TEST(MatrixInverseTest, TriangularMatrixInverseTestUnsuccessful)
{
	const Matrix toBeProcessed{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	EXPECT_THROW({(void)Inverse(toBeProcessed);}, std::runtime_error);
}

TEST(MatrixSimpleOperations, MatrixChainedOperations_1)
{
	const Matrix matrix1{
		{ 1, 7 },
		{ 6, 8 }
	};
	const Matrix matrix2{
		{ -3, 6 },
		{ 10, 2.5 }
	};

	const Matrix result{matrix1 * 8 + matrix2 * 4};
	const Matrix expected{
		{ -4, 80 },
		{ 88, 74 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixSimpleOperations, MatrixChainedOperations_2)
{
	const Matrix matrix1{
		{ 1, 7 },
		{ 6, 8 }
	};
	const Matrix matrix2{
		{ -3, 6 },
		{ 10, 2.5 }
	};
	const Matrix matrix3{
		{ 1, 0 },
		{ 0, 1 }
	};

	const Matrix result{matrix3 - matrix1 * 9  + 10 + matrix2 * 4};
	const Matrix expected{
		{ -10, -29 },
		{ -4, -51 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixSimpleOperations, MatrixChainedOperations_3)
{
	const Matrix matrix1{
		{ 1, 7 },
		{ 6, 8 }
	};
	const Matrix matrix2{
		{ -3, 6 },
		{ 10, 2.5 }
	};

	const Matrix result{-matrix1 / 4 + matrix1 + matrix2};
	const Matrix expected{
		{ -2.25, 11.25 },
		{ 14.5, 8.5 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixSimpleOperations, MatrixChainedOperations_4)
{
	const Matrix matrix1{
		{ 1, 7 },
		{ 6, 8 }
	};
	const Matrix matrix2{
		{ -3, 6 },
		{ 10, 2.5 }
	};

	const Matrix result{matrix1 * matrix2 * 10 - 5};
	const Matrix expected{
		{ 665, 230 },
		{ 615, 555 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixSimpleOperations, MatrixChainedOperations_5)
{
	const Matrix matrix1{
		{ 1, 7 },
		{ 6, 8 }
	};
	const Matrix matrix2{
		{ -3, 6 },
		{ 10, 2.5 }
	};

	const Matrix result{matrix1 * 10 * matrix2 * 10 - 5};
	const Matrix expected{
		{ 6695, 2345 },
		{ 6195, 5595 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixSimpleOperations, MatrixChainedOperations_6)
{
	const Matrix matrix1{
		{ 1 },
		{ 6 }
	};
	const Matrix matrix2{
		{ -3, 6 },
		{ 10, 2.5 }
	};

	const Matrix result{Transpose(matrix1 * 10) * matrix2 * 10 - 5};
	const Matrix expected{
		{ 5695, 2095 },
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixSimpleOperations, MatrixChainedOperations_7)
{
	Matrix matrix1{
		{ 1 },
		{ 6 }
	};
	const Matrix matrix2{
		{ -3, 6 },
		{ 10, 2.5 }
	};

	const Matrix result{Transpose(SetAll(matrix1, 0) * 10) * matrix2 * 10 - 5};
	const Matrix expected{
		{ -5, -5 },
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
TEST(MatrixSimpleOperations, MatrixChainedOperations_8)
{
	const Matrix matrix1{
		{ 1, 10 },
		{ 6, 0 }
	};
	const Matrix matrix2{
		{ -3, 6 },
		{ 10, 2.5 }
	};

	const Matrix result{Multiply(Transpose(matrix1), matrix2) * matrix2};
	const Matrix expected{
		{ 369, 72 },
		{ -300, 600 }
	};
	EXPECT_THAT(result, IsEqualMatrix(expected));
}
