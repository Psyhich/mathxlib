#ifndef MATRIX_MINOR_H
#define MATRIX_MINOR_H

#include "matrix.h"
#include "view.h"

namespace MxLib
{
	template<ReadonlyMatrixT Viewed>
	class MinorView
	{
	public:
		using contained = Viewed::contained;

		MinorView(const Viewed &matrix, std::size_t row, std::size_t col) :
			m_viewedMatrix{matrix},
			m_excludedRow{row},
			m_excludedCol{col}
		{
			if(matrix.Rows() == 0)
			{
				throw std::out_of_range{"Cannot take minor of matrix with rows count 0"};
			}
			if(matrix.Cols() == 0)
			{
				throw std::out_of_range{"Cannot take minor of matrix with cols count 0"};
			}
			if (m_excludedRow >= matrix.Rows())
			{
				throw std::out_of_range{fmt::format(
					"Cannot take minor by {} row from matrix that has {} rows", m_excludedRow, matrix.Rows())};
			}
			if (m_excludedCol >= matrix.Cols())
			{
				throw std::out_of_range{fmt::format(
					"Cannot take minor by {} col from matrix that has {} cols", m_excludedCol, matrix.Cols())};
			}
		}

		[[nodiscard]] constexpr inline size_t Rows() const noexcept { return m_viewedMatrix.Rows() - 1; }
		[[nodiscard]] constexpr inline size_t Cols() const noexcept { return m_viewedMatrix.Cols() - 1; }

		constexpr inline const contained &operator()(size_t row, size_t col) const
		{
			CheckBounds(*this, row, col);
			return m_viewedMatrix(row >= m_excludedRow ? row + 1 : row, col >= m_excludedCol ? col + 1 : col);
		}

	private:
		const Viewed &m_viewedMatrix;
		const std::size_t m_excludedRow;
		const std::size_t m_excludedCol;
	};

	static_assert(ReadonlyMatrixT<MinorView<MatrixD>>);
}

#endif // MATRIX_MINOR_H
