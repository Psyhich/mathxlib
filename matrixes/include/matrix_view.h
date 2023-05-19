#ifndef MATRIX_VIEW_H
#define MATRIX_VIEW_H

#include "matrix.h"

namespace MxLib
{
template<ReadonlyMatrixT M>
class MatrixView
{
public:
	MatrixView(const M &matrixToView,
		std::size_t rowStart, std::size_t colStart,
		std::size_t rows, std::size_t cols) :
		m_rowStart{rowStart},
		m_colStart{colStart},
		m_rows{rows},
		m_cols{cols},
		m_viewedMatrix{matrixToView}
	{
		if(rowStart + rows > m_viewedMatrix.Rows())
		{
			throw std::out_of_range{
				fmt::format("Exceeded {} rows count in viewed matrix with {} rows given",
					m_viewedMatrix.Rows(), Rows())};
		}
		if(colStart + cols > m_viewedMatrix.Cols())
		{
			throw std::out_of_range{
				fmt::format("Exceeded {} cols count in viewed matrix with {} cols given",
					m_viewedMatrix.Cols(), Cols())};
		}
	}

	[[nodiscard]] constexpr inline size_t Cols() const noexcept { return m_cols; }
	[[nodiscard]] constexpr inline size_t Rows() const noexcept { return m_rows; }

	constexpr inline double operator()(size_t row, size_t col) const
	{
		CheckBounds(*this, row, col);
		return m_viewedMatrix(m_rowStart + row, m_colStart + col);
	}

private:
	const std::size_t m_rowStart{0};
	const std::size_t m_colStart{0};
	const std::size_t m_rows{0};
	const std::size_t m_cols{0};

	const M &m_viewedMatrix;
};

static_assert(ReadonlyMatrixT<MatrixView<Matrix>>);
}

#endif // MATRIX_VIEW_H
