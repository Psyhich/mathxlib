#ifndef MATRIX_VIEW_H
#define MATRIX_VIEW_H

#include "matrix.h"

namespace MxLib
{
	template<ReadonlyMatrixT M>
	class MatrixView
	{
	public:
		using contained = M::contained;

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

		constexpr inline const contained &operator()(size_t row, size_t col) const
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

	template<ReadonlyMatrixT M>
	class RowView
	{
	public:
		using contained = M::contained;

		RowView(const M &viewedMatrix, std::size_t viewedRow) :
			m_viewedMatrix{viewedMatrix},
			m_viewedRow{viewedRow}
		{
			if (viewedRow >= viewedMatrix.Rows())
			{
				throw std::out_of_range{fmt::format(
					"Cannot take view of row {} of matrix that has {} rows", viewedRow, viewedMatrix.Rows())};
			}
		}

		[[nodiscard]] inline std::size_t Rows() const { return 1; }
		[[nodiscard]] inline std::size_t Cols() const { return m_viewedMatrix.Cols(); }

		[[nodiscard]] inline std::size_t size() const { return Cols(); }

		[[nodiscard]] inline const contained &operator[](std::size_t colToView) const
		{
			return m_viewedMatrix(m_viewedRow, colToView);
		}

		[[nodiscard]] inline const contained &operator()(std::size_t row, std::size_t col) const
		{
			CheckBounds(*this, row, col);
			return m_viewedMatrix(m_viewedRow, col);
		}


	private:
		const M &m_viewedMatrix;
		std::size_t m_viewedRow;
	};

	template<typename M>
	class ColView
	{
	public:
		using contained = M::contained;

		ColView(const M &viewedMatrix, std::size_t viewedCol) :
			m_viewedMatrix{viewedMatrix},
			m_viewedCol{viewedCol}
		{
			if (viewedCol >= viewedMatrix.Rows())
			{
				throw std::out_of_range{fmt::format(
					"Cannot take view of row {} of matrix that has {} rows", viewedCol, viewedMatrix.Rows())};
			}
		}

		[[nodiscard]] inline std::size_t Rows() const { return m_viewedMatrix.Rows(); }
		[[nodiscard]] inline std::size_t Cols() const { return 1; }
		[[nodiscard]] inline std::size_t size() const { return Rows(); }

		[[nodiscard]] inline const contained &operator[](std::size_t rowToView) const 
		{
			return m_viewedMatrix(rowToView, m_viewedCol);
		}

		[[nodiscard]] inline const contained &operator()(std::size_t row, std::size_t col) const
		{
			CheckBounds(*this, row, col);
			return m_viewedMatrix(row, m_viewedCol);
		}


	private:
		const M &m_viewedMatrix;
		std::size_t m_viewedCol;
	};

	static_assert(ReadonlyMatrixT<MatrixView<MatrixF>>);
	static_assert(ReadonlyMatrixT<MatrixView<MatrixD>>);

	static_assert(ReadonlyMatrixT<ColView<MatrixF>>);
	static_assert(ReadonlyMatrixT<ColView<MatrixD>>);

	static_assert(ReadonlyMatrixT<RowView<MatrixF>>);
	static_assert(ReadonlyMatrixT<RowView<MatrixD>>);

	static_assert(ReadonlyMatrixT<ColView<MatrixView<MatrixF>>>);
	static_assert(ReadonlyMatrixT<ColView<MatrixView<MatrixD>>>);

	static_assert(ReadonlyMatrixT<RowView<MatrixView<MatrixF>>>);
	static_assert(ReadonlyMatrixT<RowView<MatrixView<MatrixD>>>);

}

#endif // MATRIX_VIEW_H
