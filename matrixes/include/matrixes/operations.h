#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <random>

#include "matrix.h"

namespace MxLib
{
	static inline constexpr const double DEFAULT_ACCURACY{1e-6};

	template<MatrixT M>
	M &Randomise(M &matrixToRandomise, double startValue, double endValue)
	{
		using namespace std;

		static random_device generator;
		uniform_real_distribution<double> distribution(startValue, endValue);

		for (size_t row = 0; row < matrixToRandomise.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToRandomise.Cols(); col++)
			{
				matrixToRandomise(row, col) = distribution(generator);
			}
		}
		return matrixToRandomise;
	}

	template<ReadonlyMatrixT lM, ReadonlyMatrixT rM>
	inline void CheckDimensions(const lM &lMatrixToCheck, const rM& rMatrixToCheck)
	{
		if(lMatrixToCheck.Cols() != rMatrixToCheck.Cols())
		{
			throw std::length_error("Required columns not matched");
		}
		else if(lMatrixToCheck.Rows() != rMatrixToCheck.Rows())
		{
			throw std::length_error("Required rows not matched");
		}
	}

	template<ReadonlyMatrixT M>
	inline void IsSquareMatrix(const M &matrixToCheck)
	{
		if(matrixToCheck.Rows() != matrixToCheck.Cols())
		{
			throw std::runtime_error("This matrixe is not square one");
		}
	}

	template<ReadonlyMatrixT M>
	void Print(const M &matrixToPrint)
	{
		for (size_t i = 0; i < matrixToPrint.Rows(); i++)
		{
			fmt::print("| ");
			for (size_t j = 0; j < matrixToPrint.Cols(); j++)
			{
				fmt::print(" {} ", matrixToPrint(i, j));
			}
			fmt::print(" |\n");
		}
		fmt::print("\n");
	}

	template<ReadonlyMatrixT lM, ReadonlyMatrixT rM>
	[[nodiscard]] Matrix Multiply(const lM &lMatrixToMultiply,
		const rM &rMatrixToMultiply)
	{
		CheckDimensions(lMatrixToMultiply, rMatrixToMultiply);

		Matrix outMatrix{lMatrixToMultiply};
		for(size_t row = 0; row < lMatrixToMultiply.Rows(); row++)
		{
			for (size_t col = 0; col < lMatrixToMultiply.Cols(); col++)
			{
				outMatrix(row, col) *= rMatrixToMultiply(row, col);
			}
		}
		return outMatrix;
	}

	template<ReadonlyMatrixT lM, ReadonlyMatrixT rM>
	[[nodiscard]] bool IsEqualTo(const lM &lMatrix,
		const rM &rMatrix, double eps=DEFAULT_ACCURACY) noexcept
	{
		if(lMatrix.Cols() != rMatrix.Cols() ||
			lMatrix.Rows() != rMatrix.Rows())
		{
			return false;
		}
		for(std::size_t row = 0; row < lMatrix.Rows(); row++)
		{
			for(std::size_t col = 0; col < lMatrix.Cols(); col++)
			{
				if(std::abs(lMatrix(row, col) - rMatrix(row, col)) > eps)
				{
					return false;
				}
			}
		}
		return true;
	}

	template<MatrixT M>
	M &SetAll(M &matrixToSet, double valueToSet) noexcept
	{
		for(std::size_t row = 0; row < matrixToSet.Rows(); row++)
		{
			for(std::size_t col = 0; col < matrixToSet.Cols(); col++)
			{
				matrixToSet(row, col) = valueToSet;
			}
		}
		return matrixToSet;
	}

	// Custom operators
	template<ReadonlyMatrixT lM, ReadonlyMatrixT rM>
	Matrix operator*(const lM &lMatrixToDotProduct, const rM &rMatrixToDotProduct)
	{
		if(lMatrixToDotProduct.Cols() != rMatrixToDotProduct.Rows())
		{
			throw std::length_error("Columns of left matrix doesn't match rows of right one");
		}

		Matrix out{lMatrixToDotProduct.Rows(), rMatrixToDotProduct.Cols()};

		for (size_t row = 0; row < lMatrixToDotProduct.Rows(); row++)
		{
			for (size_t col = 0; col < rMatrixToDotProduct.Cols(); col++) 
			{
				double &val = out(row, col);
				val = 0;
				for (size_t iter = 0; iter < rMatrixToDotProduct.Rows(); iter++)
				{
					val += lMatrixToDotProduct(row, iter) * rMatrixToDotProduct(iter, col);
				}
			}
		}

		return out;
	}

	template<ReadonlyMatrixT lM, ReadonlyMatrixT rM>
	Matrix operator+(const lM &lMatrix, const rM &rMatrix)
	{
		CheckDimensions(lMatrix, rMatrix);

		Matrix outMatrix = lMatrix;
		for (size_t row = 0; row < lMatrix.Rows(); row++)
		{
			for (size_t col = 0; col < lMatrix.Cols(); col++)
			{
				outMatrix(row, col) += rMatrix(row, col);
			}
		}

		return outMatrix;
	}

	template<ReadonlyMatrixT lM, ReadonlyMatrixT rM>
	Matrix operator-(const lM &lMatrix, const rM &rMatrix)
	{
		CheckDimensions(lMatrix, rMatrix);

		Matrix outMatrix = lMatrix;
		for (size_t row = 0; row < lMatrix.Rows(); row++)
		{
			for (size_t col = 0; col < lMatrix.Cols(); col++)
			{
				outMatrix(row, col) -= rMatrix(row, col);
			}
		}

		return outMatrix;
	}

	template<MatrixT lM, ReadonlyMatrixT rM>
	Matrix &operator+=(lM &lMatrix, const rM &rMatrix)
	{
		CheckDimensions(lMatrix, rMatrix);

		for (size_t row = 0; row < lMatrix.Rows(); row++)
		{
			for (size_t col = 0; col < lMatrix.Cols(); col++)
			{
				lMatrix(row, col) += rMatrix(row, col);
			}
		}
		return lMatrix;
	}

	template<MatrixT lM, ReadonlyMatrixT rM>
	Matrix &operator-=(lM &lMatrix, const rM &rMatrix)
	{
		CheckDimensions(lMatrix, rMatrix);

		for (size_t row = 0; row < lMatrix.Rows(); row++)
		{
			for (size_t col = 0; col < lMatrix.Cols(); col++)
			{
				lMatrix(row, col) -= rMatrix(row, col);
			}
		}
		return lMatrix;
	}

	// TODO: add scalar assigment operations
	// Operations with scalar values
	template<ReadonlyMatrixT M>
	Matrix operator/(const M &matrixToChange, double numberToDivide)
	{
		Matrix outMatrix = matrixToChange;
		for (size_t row = 0; row < matrixToChange.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToChange.Cols(); col++)
			{
				outMatrix(row, col) /= numberToDivide;
			}
		}
		return outMatrix;
	}

	template<ReadonlyMatrixT M>
	Matrix operator*(const M &matrixToChange, double numberToMultiply)
	{
		Matrix outMatrix = matrixToChange;
		for (size_t row = 0; row < matrixToChange.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToChange.Cols(); col++)
			{
				outMatrix(row, col) *= numberToMultiply;
			}
		}
		return outMatrix;
	}

	template<ReadonlyMatrixT M>
	Matrix operator+(const M &matrixToChange, double numberToAdd)
	{
		Matrix outMatrix = matrixToChange;
		for (size_t row = 0; row < matrixToChange.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToChange.Cols(); col++)
			{
				outMatrix(row, col) += numberToAdd;
			}
		}
		return outMatrix;
	}

	template<ReadonlyMatrixT M>
	Matrix operator-(const M &matrixToChange, double numberToSubtract)
	{
		Matrix outMatrix = matrixToChange;
		for (size_t row = 0; row < matrixToChange.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToChange.Cols(); col++)
			{
				outMatrix(row, col) -= numberToSubtract;
			}
		}
		return outMatrix;
	}

	// Unaries
	template<ReadonlyMatrixT M>
	Matrix operator-(const M& matrixToChange)
	{
		Matrix negativeMatrix{matrixToChange};
		for (size_t row = 0; row < matrixToChange.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToChange.Cols(); col++)
			{
				negativeMatrix(row, col) *= -1;
			}
		}
		return negativeMatrix;
	}

}

#endif // MATRIX_OPERATIONS_H
