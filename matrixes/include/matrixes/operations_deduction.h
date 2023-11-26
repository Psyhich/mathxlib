#ifndef OPERATIONS_DEDUCTION_H
#define OPERATIONS_DEDUCTION_H

#include "matrix_concepts.h"

namespace MxLib
{
	template<MatrixT M>
	struct MatrixConstructor
	{
		static constexpr M Create(std::size_t rows, std::size_t cols)
		{
			return M{rows, cols};
		}
	};

	template<MatrixT M>
	struct TransposeOperationDeducer
	{
		using value = M;
	};


	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix, typename ResultT>
	struct OperationDeducer
	{
		using contained = ResultT;
		using value = LMatrix;
	};

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix, typename ResultT>
	using OperationResult = typename OperationDeducer<LMatrix, RMatrix, ResultT>::value;

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix, typename ResultT>
	struct ScalarOperationDeducer
	{
		using contained = ResultT;
		using value = OperationResult<LMatrix, RMatrix, ResultT>;
	};

	template<MatrixT Matrix>
	using TransposeResult = typename TransposeOperationDeducer<Matrix>::value;

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix, typename ResultT>
	using ScalarResult = typename ScalarOperationDeducer<RMatrix, LMatrix, ResultT>::value;

	template<ReadonlyMatrixT LMatrix>
	using NegativeResult = ScalarResult<LMatrix, LMatrix, decltype(-typename LMatrix::contained{})>;

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix=LMatrix>
	using SumResult = ScalarResult<LMatrix, RMatrix, decltype(typename LMatrix::contained{} + typename RMatrix::contained{})>;

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix=LMatrix>
	using SubtractionResult = ScalarResult<LMatrix, RMatrix, decltype(typename LMatrix::contained{} - typename RMatrix::contained{})>;

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix=LMatrix>
	using MultiplicationResult = ScalarResult<LMatrix, RMatrix, decltype(typename LMatrix::contained{} * typename RMatrix::contained{})>;

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix=LMatrix>
	using DivisionResult = ScalarResult<LMatrix, RMatrix, decltype(typename LMatrix::contained{} / typename RMatrix::contained{})>;


	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix>
	struct DotProductResultDeducer
	{
		using contained = decltype(typename LMatrix::contained{} * typename RMatrix::contained{}
			+ typename LMatrix::contained{} * typename RMatrix::contained{});
		using value = OperationResult<LMatrix, RMatrix, contained>;
	};

	template<ReadonlyMatrixT LMatrix, ReadonlyMatrixT RMatrix=LMatrix>
	using DotProductResult = typename DotProductResultDeducer<LMatrix, RMatrix>::value;
}
#endif // OPERATIONS_DEDUCTION_H
