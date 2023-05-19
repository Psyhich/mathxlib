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
		{ possibleMatrix(1, 1) } -> std::convertible_to<const double&>;

		// Dimensions getters
		{ possibleMatrix.Cols() } -> std::same_as<std::size_t>;
		{ possibleMatrix.Rows() } -> std::same_as<std::size_t>;
	};

template<typename T>
concept MatrixT = ReadonlyMatrixT<T> &&
	requires(T possibleMatrix)
	{
		// Setters
		{ possibleMatrix(1, 1) } -> std::convertible_to<double&>;
	};

template<typename T>
concept ContiniousStorageMatrix = MatrixT<T> &&
	requires(T possibleContiniousMatrix)
	{
		{ possibleContiniousMatrix.data() } -> std::convertible_to<const double *>;
		{ possibleContiniousMatrix.size() } -> std::same_as<std::size_t>;
	};

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
