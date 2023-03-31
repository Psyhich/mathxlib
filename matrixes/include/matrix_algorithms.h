#ifndef MATRIX_ALGORITHM_H
#define MATRIX_ALGORITHM_H

#include <random>
#include <utility>
#include <stack>

#include <fmt/format.h>

#include "matrix_operations.h"


namespace MxLib::algo
{
	using MapFunc = std::function<void(double &value)>;

	template<MatrixT M>
	[[nodiscard]] Matrix Map(const M& matrix, const MapFunc &func)
	{
		Matrix outMatrix{matrix};
		for (size_t row = 0; row < outMatrix.Rows(); row++)
		{
			for (size_t col = 0; col < outMatrix.Cols(); col++)
			{
				func(outMatrix(row, col));
			}
		}

		return outMatrix;
	}

	template<MatrixT M>
	M &MapInplace(M &matrixToMap, const MapFunc &func)
	{
		for (size_t row = 0; row < matrixToMap.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToMap.Cols(); col++)
			{
				func(matrixToMap(row, col));
			}
		}
	}

	template<MatrixT M>
	[[nodiscard]] Matrix Transpose(const M &matrixToTranspose)
	{
		Matrix transposed(matrixToTranspose.Cols(), matrixToTranspose.Rows());
		for (size_t row = 0; row < matrixToTranspose.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToTranspose.Cols(); col++)
			{
				transposed(col, row) = matrixToTranspose(row, col);
			}
		}
		return transposed;
	}

	template<MatrixT M>
	Matrix ConstructMinor(const M &matrixToSet, const size_t rowToExclude, const size_t colToExclude)
	{
		Matrix minor{matrixToSet.Rows() - 1, matrixToSet.Cols() - 1};
		size_t rowIndex{0};
		for (size_t row = 0; row < matrixToSet.Rows(); row++)
		{
			if(row != rowToExclude)
			{
				size_t colIndex{0};
				for (size_t col = 0; col < matrixToSet.Cols(); col++)
				{
					if (col != colToExclude)
					{
						minor(rowIndex, colIndex) = matrixToSet(row, col);
						++colIndex;
					}
				}
				++rowIndex;
			}
		}
		return minor;
	}

	template<MatrixT M>
	[[nodiscard]] double Determinant(const M &matrix)
	{
		IsSquareMatrix(matrix);

		std::stack<std::pair<double, Matrix>> matrixesToCalculate;
		matrixesToCalculate.push({ 1, matrix });

		double determinant = 0;
		while(!matrixesToCalculate.empty())
		{
			const double coefficient = matrixesToCalculate.top().first;
			const Matrix currentMatrix = std::move(matrixesToCalculate.top().second);
			matrixesToCalculate.pop();

			const size_t currentMatrixRank = currentMatrix.Rows();
			if(currentMatrixRank == 1)
			{
				determinant += coefficient * currentMatrix(0, 0);
			}
			else if(currentMatrixRank == 2)
			{
				determinant += coefficient * (currentMatrix(0, 0) * currentMatrix(1, 1) -
					currentMatrix(0, 1) * currentMatrix(1, 0));
			}
			else
			{
				for(size_t i = 0; i < currentMatrixRank; i++)
				{
					matrixesToCalculate.push(
						{
							currentMatrix(0, i) * (i % 2 == 0 ? 1 : -1),
							ConstructMinor(currentMatrix, 0, i)
						});
				}
			}
		}

		return determinant;
	}


	template<MatrixT M>
	[[nodiscard]] Matrix Adjoint(const M &matrix)
	{
		IsSquareMatrix(matrix);

		Matrix outMatrix{matrix.Rows()};
		size_t const rank = matrix.Rows();
		for (size_t currRow = 0; currRow < rank; currRow++)
		{
			for (size_t currCol = 0; currCol < rank; currCol++)
			{
				Matrix minor = ConstructMinor(matrix, currRow, currCol);
				// After we constructed minor calculating it's cofactor and assigning
				// it to outMatrix
				outMatrix(currRow, currCol) = Determinant(minor) * ((currRow + currCol) % 2 == 0 ? 1 : -1);
			}
		}

		return Transpose(outMatrix);
	}
	
	template<MatrixT M>
	[[nodiscard]] Matrix Inverse(const M &matrixToInverse)
	{
		IsSquareMatrix(matrixToInverse);

		const double determinant = Determinant(matrixToInverse);
		if(std::abs(determinant) <= DEFAULT_ACCURACY)
		{
			throw std::runtime_error("Matrix is triangular, no inverse can be found");
		}
		return Transpose(Transpose(Adjoint(matrixToInverse)) / determinant);
	}
}

#endif // MATRIX_ALGORITHM_H
