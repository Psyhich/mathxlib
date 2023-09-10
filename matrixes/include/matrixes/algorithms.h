#ifndef MATRIX_ALGORITHM_H
#define MATRIX_ALGORITHM_H

#include <random>
#include <utility>
#include <stack>
#include <functional>

#include <fmt/format.h>

#include "operations.h"
#include "minor.h"


namespace MxLib::algo
{
	template<typename ReturnT, typename InT>
	using MapFunc = std::function<ReturnT(InT &value)>;

	template<ReadonlyMatrixT M, typename OutT>
	[[nodiscard]] ArithmeticResult<M, M, OutT> Map(const M& matrix, const MapFunc<OutT, typename M::contained> &func)
	{
		ArithmeticResult<M, M, OutT> outMatrix{matrix};
		for (size_t row = 0; row < outMatrix.Rows(); row++)
		{
			for (size_t col = 0; col < outMatrix.Cols(); col++)
			{
				func(outMatrix(row, col));
			}
		}

		return outMatrix;
	}

	template<MatrixT M, typename OutT>
	M &MapInplace(M &matrixToMap, const MapFunc<OutT, typename M::contained> &func)
	{
		for (size_t row = 0; row < matrixToMap.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToMap.Cols(); col++)
			{
				func(matrixToMap(row, col));
			}
		}
	}

	template<ReadonlyMatrixT M>
	[[nodiscard]] ArithmeticResult<M, M, typename M::contained> Transpose(const M &matrixToTranspose)
	{
		ArithmeticResult<M, M, typename M::contained> transposed{matrixToTranspose.Cols(), matrixToTranspose.Rows()};
		for (size_t row = 0; row < matrixToTranspose.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToTranspose.Cols(); col++)
			{
				transposed(col, row) = matrixToTranspose(row, col);
			}
		}
		return transposed;
	}

	template<ReadonlyMatrixT M>
	[[nodiscard]] typename M::contained Determinant(const M &matrix)
	{
		using ContainedT = typename M::contained;
		using OutMatrix = ArithmeticResult<M, M, ContainedT>;

		IsSquareMatrix(matrix);

		std::stack<std::pair<ContainedT, OutMatrix>> matrixesToCalculate;
		matrixesToCalculate.emplace(1, OutMatrix{matrix});

		ContainedT determinant = 0;
		while(!matrixesToCalculate.empty())
		{
			const ContainedT coefficient = matrixesToCalculate.top().first;
			const OutMatrix currentMatrix = std::move(matrixesToCalculate.top().second);
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
				for(std::size_t i = 0; i < currentMatrixRank; i++)
				{
					matrixesToCalculate.emplace(
						currentMatrix(0, i) * (i % 2 == 0 ? 1 : -1),
						MinorView(currentMatrix, 0, i)
					);
				}
			}
		}

		return determinant;
	}

	template<ReadonlyMatrixT M>
	[[nodiscard]] MultiplicationResult<M> Adjoint(const M &matrix)
	{
		IsSquareMatrix(matrix);

		MultiplicationResult<M> outMatrix{matrix.Rows()};
		const size_t rank = matrix.Rows();
		for (size_t currRow = 0; currRow < rank; currRow++)
		{
			for (size_t currCol = 0; currCol < rank; currCol++)
			{
				const MinorView minor{matrix, currRow, currCol};
				// After we constructed minor calculating it's cofactor and assigning
				// it to outMatrix
				outMatrix(currRow, currCol) = Determinant(minor) * ((currRow + currCol) % 2 == 0 ? 1 : -1);
			}
		}

		return Transpose(outMatrix);
	}
	
	template<ReadonlyMatrixT M>
	[[nodiscard]] DivisionResult<MultiplicationResult<M>> Inverse(const M &matrixToInverse)
	{
		using ContainedT = typename DivisionResult<MultiplicationResult<M>>::contained;
		IsSquareMatrix(matrixToInverse);

		const ContainedT determinant = Determinant(matrixToInverse);
		if(std::abs(determinant) <= DEFAULT_ACCURACY)
		{
			throw std::runtime_error("Matrix is triangular, no inverse can be found");
		}
		return Transpose(Transpose(Adjoint(matrixToInverse)) / determinant);
	}
}

#endif // MATRIX_ALGORITHM_H
