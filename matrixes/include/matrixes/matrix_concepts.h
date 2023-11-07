#ifndef MATRIX_CONCEPT_H
#define MATRIX_CONCEPT_H

#include <concepts>
#include <type_traits>
#include <stdexcept>

#include <fmt/format.h>

template<typename T>
concept ReadonlyMatrixT =
	requires(T possibleMatrix)
	{
		{ possibleMatrix(1, 1) } -> std::convertible_to<const typename T::contained&>;

		// Dimensions getters
		{ possibleMatrix.Cols() } -> std::convertible_to<std::size_t>;
		{ possibleMatrix.Rows() } -> std::convertible_to<std::size_t>;
	};

template<typename T>
concept MatrixT = ReadonlyMatrixT<T> &&
	requires(T possibleMatrix)
	{
		{ possibleMatrix(1, 1) } -> std::convertible_to<typename T::contained&>;
	};

template<typename T>
concept ContinuousStorageMatrix = MatrixT<T> &&
	requires(T possibleContinuousMatrix)
	{
		{ possibleContinuousMatrix.data() } -> std::convertible_to<const typename T::contained *>;
		{ possibleContinuousMatrix.size() } -> std::convertible_to<std::size_t>;
	};

template<typename T>
concept StaticMatrixT = ReadonlyMatrixT<T> &&
	std::convertible_to<std::size_t, typename T::COLS> &&
	std::convertible_to<std::size_t, typename T::ROWS>;

template<typename M1, typename M2>
concept AllowedVectorizedAction = StaticMatrixT<M1> && StaticMatrixT<M2> &&
	M1::COLS == M2::COLS && M1::ROWS == M2::ROWS;

template<typename M1, typename M2>
concept AllowedDotProductAction = StaticMatrixT<M1> && StaticMatrixT<M2> &&
	M1::COLS == M2::ROWS;

namespace MxLib
{
	template<ReadonlyMatrixT M>
	constexpr void CheckBounds(const M &matrixToCheck, std::size_t row, std::size_t col)
	{
		if(row >= matrixToCheck.Rows() || col >= matrixToCheck.Cols())
		{
			throw std::out_of_range{
				fmt::format("Reached out of bounds of matrix view {}x{} with row: {} and col: {}",
					matrixToCheck.Rows(), matrixToCheck.Cols(), row, col)};
		}
	}
}


#endif // MATRIX_CONCEPT_H
