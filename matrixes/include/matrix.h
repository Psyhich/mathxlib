#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <stdexcept>

#include <fmt/format.h>

#include "matrix_concepts.h"

namespace MxLib
{

class Matrix
{
public:
	using size_t = std::size_t;

	Matrix() noexcept = default;

	template<size_t ColsCount>
	explicit Matrix(const std::initializer_list<double[ColsCount]> &initializer) :
		m_cols{ColsCount},
		m_rows{initializer.size()},
		m_size{m_rows * m_cols},
		m_values{new double[m_size]}
	{
		for(std::size_t rowIndex = 0; rowIndex < m_rows; rowIndex++)
		{
			for(std::size_t colIndex = 0; colIndex < ColsCount; colIndex++)
			{
				(*this)(rowIndex, colIndex) = initializer.begin()[rowIndex][colIndex];
			}
		}
	}
	Matrix(size_t rows, size_t cols);
	explicit Matrix(size_t dimensions);

	Matrix(const double *data, size_t rows, size_t cols);
	Matrix(const double **data, size_t rows, size_t cols);

	Matrix(const Matrix &matrixToCopy);
	Matrix &operator=(const Matrix &matrixToCopy);

	Matrix(Matrix &&matrixToMove) noexcept;
	Matrix &operator=(Matrix &&matrixToMove) noexcept;

	~Matrix() noexcept;

	// Getters and setters
	[[nodiscard]] constexpr inline size_t Cols() const noexcept { return m_cols; }
	[[nodiscard]] constexpr inline size_t Rows() const noexcept { return m_rows; }

	constexpr inline const double &operator()(size_t row, size_t col) const
	{
		CheckBounds(*this, row, col);
		return m_values[CalculatePos(row, col)];
	}
	constexpr inline double &operator()(size_t row, size_t col)
	{
		CheckBounds(*this, row, col);
		return m_values[CalculatePos(row, col)];
	}

	[[nodiscard]] constexpr inline double *data()
	{
		return m_values;
	}
	[[nodiscard]] constexpr inline const double *data() const
	{
		return m_values;
	}

	[[nodiscard]] constexpr inline std::size_t size() const
	{
		return m_size;
	}

private:
	void MoveData(Matrix &&matrixToMove) noexcept;
	void CopyData(const Matrix& matrixToCopy);

	[[nodiscard]] constexpr inline size_t CalculatePos(size_t row, size_t col) const noexcept
	{
		return row * m_cols + col;
	}

	size_t m_cols{ 0 };
	size_t m_rows{ 0 };
	size_t m_size{ 0 };

	double *m_values{ nullptr };
};

static_assert(MatrixT<Matrix>, "Basic matrix type doesn't follow the MatrixT concept");
static_assert(ContiniousStorageMatrix<Matrix>, "Basic matrix type doesn't follow the MatrixT concept");

}

#endif // MATRIX_H
