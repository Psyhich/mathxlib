#include "matrixes/algorithms.h"

MxLib::Matrix Elimination(MxLib::Matrix &matr, std::size_t pivotRow, std::size_t pivotCol)
{
	const std::size_t cols{matr.Cols()};

	--pivotRow;
	pivotCol = (pivotCol == cols - 1) ? cols - 2 : pivotCol - 1;

	while (pivotRow != 0 && pivotCol != 0)
	{
		if (std::abs(matr(pivotRow, pivotCol)) <= std::numeric_limits<double>::epsilon())
		{
			--pivotCol;
			--pivotRow;
			continue;
		}

		matr(pivotRow, cols - 1) /= matr(pivotRow, pivotCol);

		for (std::size_t row = pivotRow; row-- != 0;)
		{
			const double coeff{matr(row, pivotCol)};
			matr(row, cols - 1) -= coeff * matr(pivotRow, cols - 1);
			matr(row, pivotCol) = 0;
		}
		matr(pivotRow, pivotCol) = 1;

		--pivotCol;
		--pivotRow;
	}

	MxLib::Matrix solution{1, matr.Cols() - 1};
	for (std::size_t index = 0; index < matr.Rows(); ++index)
	{
		solution(0, index) = matr(index, cols - 1) / matr(index, index);
	}

	return solution;
}

namespace MxLib::algo
{
	[[nodiscard]] Matrix SolveGaussJordan(Matrix &augmentedMatrix)
	{
		const std::size_t rows{augmentedMatrix.Rows()};
		const std::size_t cols{augmentedMatrix.Cols()};

		std::size_t pivotRow{0};
		std::size_t pivotCol{0};

		while (pivotRow < rows && pivotCol < cols)
		{
			std::size_t maxRow{pivotRow};
			for (std::size_t currRow = maxRow + 1; currRow < rows; ++currRow)
			{
				if (augmentedMatrix(currRow, pivotCol) > augmentedMatrix(maxRow, pivotCol))
				{
					maxRow = currRow;
				}
			}

			if (std::abs(augmentedMatrix(maxRow, pivotCol)) <= std::numeric_limits<double>::epsilon())
			{
				throw std::domain_error("Matrix is singular");
			}

			for (std::size_t col = 0; col < cols; ++col)
			{
				std::swap(augmentedMatrix(pivotRow, col), augmentedMatrix(maxRow, col));
			}

			for (std::size_t i = pivotRow + 1; i < rows; ++i)
			{
				const double coeff{augmentedMatrix(i, pivotCol) / augmentedMatrix(pivotRow, pivotCol)};
				augmentedMatrix(i, pivotCol) = 0;

				for (std::size_t j = pivotCol + 1; j < cols; ++j)
				{
					augmentedMatrix(i, j) -= augmentedMatrix(pivotRow, j) * coeff;
				}
			}

			++pivotRow;
			++pivotCol;
		}


		return Elimination(augmentedMatrix, pivotRow, pivotCol);
	}
}
