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
	Matrix() noexcept = default;

	explicit Matrix(const std::initializer_list<std::initializer_list<double>> &initializer) :
		m_cols{initializer.size() > 0 ? initializer.begin()[0].size() : 0},
		m_rows{initializer.size()},
		m_size{m_rows * m_cols},
		m_values{m_size > 0 ? new double[m_size] : nullptr}
	{
		for(std::size_t rowIndex = 0; rowIndex < m_rows; rowIndex++)
		{
			if (initializer.begin()[rowIndex].size() != m_cols)
			{
				throw std::out_of_range{"Matrix contains inconsistant amount of columns"};
			}

			for (std::size_t colIndex = 0; colIndex < m_cols; colIndex++)
			{
				(*this)(rowIndex, colIndex) = initializer.begin()[rowIndex].begin()[colIndex];
			}
		}
	}
	Matrix(std::size_t rows, std::size_t cols);
	explicit Matrix(std::size_t dimensions);

	template<ReadonlyMatrixT M>
	explicit Matrix(const M &matrixToCopy) :
		m_cols{matrixToCopy.Cols()},
		m_rows{matrixToCopy.Rows()},
		m_size{m_cols * m_rows},
		m_values{m_size > 0 ? new double[m_size] : nullptr}
	{
		for (std::size_t row = 0; row < m_rows; ++row)
		{
			for (std::size_t col = 0; col < m_cols; ++col)
			{
				(*this)(row, col) = matrixToCopy(row, col);
			}
		}
	}

	Matrix(const double *data, std::size_t rows, std::size_t cols);
	Matrix(const double **data, std::size_t rows, std::size_t cols);

	Matrix(const Matrix &matrixToCopy);
	Matrix &operator=(const Matrix &matrixToCopy);

	Matrix(Matrix &&matrixToMove) noexcept;
	Matrix &operator=(Matrix &&matrixToMove) noexcept;

	~Matrix() noexcept;

	// Getters and setters
	[[nodiscard]] constexpr inline std::size_t Cols() const noexcept { return m_cols; }
	[[nodiscard]] constexpr inline std::size_t Rows() const noexcept { return m_rows; }

	constexpr inline const double &operator()(std::size_t row, std::size_t col) const
	{
		CheckBounds(*this, row, col);
		return m_values[CalculatePos(row, col)];
	}
	constexpr inline double &operator()(std::size_t row, std::size_t col)
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

	static Matrix Identity(std::size_t rank);

private:
	void MoveData(Matrix &&matrixToMove) noexcept;
	void CopyData(const Matrix& matrixToCopy);

	[[nodiscard]] constexpr inline std::size_t CalculatePos(std::size_t row, std::size_t col) const noexcept
	{
		return row * m_cols + col;
	}

	std::size_t m_cols{ 0 };
	std::size_t m_rows{ 0 };
	std::size_t m_size{ 0 };

	double *m_values{ nullptr };
};

static_assert(MatrixT<Matrix>, "Basic matrix type doesn't follow the MatrixT concept");
static_assert(ContiniousStorageMatrix<Matrix>, "Basic matrix type doesn't follow the ContiniousStorageMatrix concept");

}

#endif // MATRIX_H
