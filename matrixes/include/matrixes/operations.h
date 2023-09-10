#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include <random>

#include "matrix.h"

namespace MxLib
{
	static inline constexpr const double DEFAULT_ACCURACY{1e-6};

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix, typename ResultT>
	struct ArithmeticOperationDeducer
	{
		using value = Matrix<ResultT>;
	};

	template<ReadonlyMatrixT LMatrix, typename ContainedRT,
		template<typename> typename RMatrix, typename ResultT>
			requires MatrixT<RMatrix<ContainedRT>>
				&& MatrixT<RMatrix<ResultT>>
	struct ArithmeticOperationDeducer<LMatrix, RMatrix<ContainedRT>, ResultT>
	{
		using value = RMatrix<ResultT>;
	};

	template<typename ContainedLT, template<typename> typename LMatrix,
		ReadonlyMatrixT RMatrix, typename ResultT>
			requires MatrixT<LMatrix<ContainedLT>>
				&& MatrixT<LMatrix<ResultT>>
	struct ArithmeticOperationDeducer<LMatrix<ContainedLT>, RMatrix, ResultT>
	{
		using value = LMatrix<ResultT>;
	};

	template<typename ContainedLT, template<typename> typename LMatrix,
		typename ContainedRT, template<typename> typename RMatrix, typename ResultT>
			requires MatrixT<LMatrix<ContainedLT>>
				&& MatrixT<LMatrix<ResultT>>
				&& MatrixT<RMatrix<ContainedRT>>
				&& MatrixT<Matrix<ResultT>>
	struct ArithmeticOperationDeducer<LMatrix<ContainedLT>, RMatrix<ContainedRT>, ResultT>
	{
		using value = LMatrix<ResultT>;
	};

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix, typename ResultT>
	using ArithmeticResult = typename ArithmeticOperationDeducer<RMatrix, LMatrix, ResultT>::value;

	template<ReadonlyMatrixT LMatrix>
	using NegativeResult = ArithmeticResult<LMatrix, LMatrix, decltype(-typename LMatrix::contained{})>;

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix=LMatrix>
	using SumResult = ArithmeticResult<LMatrix, RMatrix, decltype(typename LMatrix::contained{} + typename RMatrix::contained{})>;

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix=LMatrix>
	using SubtractionResult = ArithmeticResult<LMatrix, RMatrix, decltype(typename LMatrix::contained{} - typename RMatrix::contained{})>;

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix=LMatrix>
	using MultiplicationResult = ArithmeticResult<LMatrix, RMatrix, decltype(typename LMatrix::contained{} * typename RMatrix::contained{})>;

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix=LMatrix>
	using DivisionResult = ArithmeticResult<LMatrix, RMatrix, decltype(typename LMatrix::contained{} / typename RMatrix::contained{})>;

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix=LMatrix>
	using DotProductResult = ArithmeticResult<LMatrix, RMatrix,
		decltype(typename LMatrix::contained{} * typename RMatrix::contained{}
			+ typename LMatrix::contained{} * typename RMatrix::contained{})>;


	template<MatrixT M>
		requires std::floating_point<typename M::contained>
	M &Randomize(M &matrixToRandomize, typename M::contained startValue, typename M::contained endValue)
	{
		using namespace std;

		static random_device generator;
		uniform_real_distribution<typename M::contained> distribution(startValue, endValue);

		for (size_t row = 0; row < matrixToRandomize.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToRandomize.Cols(); col++)
			{
				matrixToRandomize(row, col) = distribution(generator);
			}
		}
		return matrixToRandomize;
	}

	template<ReadonlyMatrixT lM, ReadonlyMatrixT rM>
	inline void CheckDimensions(const lM &lMatrixToCheck, const rM &rMatrixToCheck)
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
			throw std::runtime_error("This matrix is not square one");
		}
	}

	template<ReadonlyMatrixT lM, ReadonlyMatrixT rM>
	[[nodiscard]] MultiplicationResult<lM, rM> Multiply(const lM &lMatrixToMultiply, const rM &rMatrixToMultiply)
	{
		CheckDimensions(lMatrixToMultiply, rMatrixToMultiply);

		MultiplicationResult<lM, rM> outMatrix{lMatrixToMultiply};
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
		requires (std::integral<typename lM::contained> || std::floating_point<typename lM::contained>)
			&& (std::integral<typename rM::contained> || std::floating_point<typename rM::contained>)
	[[nodiscard]] bool IsEqualTo(const lM &lMatrix, const rM &rMatrix, double eps=DEFAULT_ACCURACY) noexcept
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
	M &SetAll(M &matrixToSet, const typename M::contained &valueToSet) noexcept
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
	DotProductResult<lM, rM> operator*(const lM &lMatrixToDotProduct, const rM &rMatrixToDotProduct)
	{
		using ContainedT = typename DotProductResult<lM, rM>::contained;
		if(lMatrixToDotProduct.Cols() != rMatrixToDotProduct.Rows())
		{
			throw std::length_error("Columns of left matrix doesn't match rows of right one");
		}

		DotProductResult<lM, rM> out{lMatrixToDotProduct.Rows(), rMatrixToDotProduct.Cols()};
		for (size_t row = 0; row < lMatrixToDotProduct.Rows(); row++)
		{
			for (size_t col = 0; col < rMatrixToDotProduct.Cols(); col++) 
			{
				ContainedT &val = out(row, col);
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
	SumResult<lM, rM> operator+(const lM &lMatrix, const rM &rMatrix)
	{
		CheckDimensions(lMatrix, rMatrix);

		SumResult<lM, rM> outMatrix{lMatrix};
		for (size_t row = 0; row < lMatrix.Rows(); row++)
		{
			for (size_t col = 0; col < lMatrix.Cols(); col++)
			{
				outMatrix(row, col) = lMatrix(row, col) + rMatrix(row, col);
			}
		}

		return outMatrix;
	}

	template<ReadonlyMatrixT lM, ReadonlyMatrixT rM>
	SubtractionResult<lM, rM> operator-(const lM &lMatrix, const rM &rMatrix)
	{
		CheckDimensions(lMatrix, rMatrix);

		SubtractionResult<lM, rM> outMatrix{lMatrix};
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
	lM &operator+=(lM &lMatrix, const rM &rMatrix)
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
	lM &operator-=(lM &lMatrix, const rM &rMatrix)
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

	// TODO: add scalar assignment operations
	// Operations with scalar values
	template<ReadonlyMatrixT M, typename T>
	ArithmeticResult<M, M, decltype(typename M::contained{} / T{})> operator/(const M &matrixToChange, const T &numberToDivide)
	{
		ArithmeticResult<M, M, decltype(typename M::contained{} / T{})> outMatrix{matrixToChange};
		for (size_t row = 0; row < matrixToChange.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToChange.Cols(); col++)
			{
				outMatrix(row, col) /= numberToDivide;
			}
		}
		return outMatrix;
	}

	template<ReadonlyMatrixT M, typename T>
	ArithmeticResult<M, M, decltype(typename M::contained{} * T{})> operator*(const M &matrixToChange, const T &numberToMultiply)
	{
		ArithmeticResult<M, M, decltype(typename M::contained{} * T{})> outMatrix{matrixToChange};
		for (size_t row = 0; row < matrixToChange.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToChange.Cols(); col++)
			{
				outMatrix(row, col) *= numberToMultiply;
			}
		}
		return outMatrix;
	}

	template<ReadonlyMatrixT M, typename T>
	ArithmeticResult<M, M, decltype(typename M::contained{} + T{})> operator+(const M &matrixToChange, const T &numberToAdd)
	{
		ArithmeticResult<M, M, decltype(typename M::contained{} + T{})> outMatrix{matrixToChange};
		for (size_t row = 0; row < matrixToChange.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToChange.Cols(); col++)
			{
				outMatrix(row, col) += numberToAdd;
			}
		}
		return outMatrix;
	}

	template<ReadonlyMatrixT M, typename T>
	ArithmeticResult<M, M, decltype(typename M::contained{} - T{})> operator-(const M &matrixToChange, const T &numberToSubtract)
	{
		ArithmeticResult<M, M, decltype(typename M::contained{} - T{})> outMatrix{matrixToChange};
		for (size_t row = 0; row < matrixToChange.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToChange.Cols(); col++)
			{
				outMatrix(row, col) -= numberToSubtract;
			}
		}
		return outMatrix;
	}

	// Unary
	template<ReadonlyMatrixT M>
	NegativeResult<M> operator-(const M &matrixToChange)
	{
		NegativeResult<M> negativeMatrix{matrixToChange};
		for (size_t row = 0; row < matrixToChange.Rows(); row++)
		{
			for (size_t col = 0; col < matrixToChange.Cols(); col++)
			{
				negativeMatrix(row, col) = -negativeMatrix(row, col);
			}
		}
		return negativeMatrix;
	}
}

#endif // MATRIX_OPERATIONS_H
