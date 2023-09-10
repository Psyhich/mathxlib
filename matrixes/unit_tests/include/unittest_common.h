#ifndef UNITTEST_COMMON_H
#define UNITTEST_COMMON_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fmt/color.h>

#include "matrixes/matrix.h"

MATCHER_P(IsEqualMatrix, rMatrix, "")
{
	if(arg.Rows() != rMatrix.Rows())
	{
		*result_listener << fmt::format(fg(fmt::color::red), "\nERROR:") <<
			fmt::format("\nMatrix has {} rows, but {} expected\n", arg.Rows(), rMatrix.Rows());
		return false;
	}
	if(arg.Cols() != rMatrix.Cols())
	{
		*result_listener << fmt::format(fg(fmt::color::red), "\nERROR:") <<
			fmt::format("\nMatrix has {} columns, but {} expected\n", arg.Cols(), rMatrix.Cols());
		return false;
	}

	constexpr const double DEFAULT_ACCURACY{1e-6};
	for(std::size_t row = 0;
		row < rMatrix.Rows(); row++)
	{
		for(std::size_t col = 0;
			col < rMatrix.Cols(); col++)
		{
			const auto difference{std::abs(arg(row, col) - rMatrix(row, col))};
			if(difference > DEFAULT_ACCURACY)
			{
				*result_listener << fmt::format(fg(fmt::color::red), "\nERROR:") <<
					fmt::format("\nGiven matrix value {} on {{{};{}}} "
							"is not nearly equals to {}.\n"
							"Difference is: {}\n",
							arg(row, col), row, col,
							rMatrix(row, col),
							difference);
				return false;
			}
		}
	}
	return testing::AssertionSuccess();
}

#endif // UNITTEST_COMMON_H
