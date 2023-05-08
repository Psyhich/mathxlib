#ifndef MATRIX_CONCEPT_H
#define MATRIX_CONCEPT_H

#include <concepts>
#include <type_traits>

template<typename T>
concept MatrixT = std::is_default_constructible_v<T> &&
	requires(T possibleMatrix)
	{
		// Getters
		{ possibleMatrix(1, 1) } -> std::convertible_to<const double&>;
		{ possibleMatrix(1, 1) } -> std::convertible_to<double&>;

		// Dimensions getters
		{ possibleMatrix.Cols() } -> std::same_as<std::size_t>;
		{ possibleMatrix.Rows() } -> std::same_as<std::size_t>;
	};

template<typename T>
concept ContiniousStorageMatrix = MatrixT<T> &&
	requires(T possibleContiniousMatrix)
	{
		{ possibleContiniousMatrix.data() } -> std::convertible_to<const double *>;
		{ possibleContiniousMatrix.size() } -> std::same_as<std::size_t>;
	};


#endif // MATRIX_CONCEPT_H
