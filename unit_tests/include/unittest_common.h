#ifndef UNITTEST_COMMON_H
#define UNITTEST_COMMON_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fmt/color.h>

#include "matrixes.h"

MATCHER_P(IsEqualMatrix, rmatrix, "")
{
	if(arg.Rows() != rmatrix.Rows())
	{
		*result_listener << fmt::format(fg(fmt::color::red), "\nERROR:") <<
			fmt::format("\nMatrix has {} rows, but {} expected\n", arg.Rows(), rmatrix.Rows());
		return false;
	}
	if(arg.Cols() != rmatrix.Cols())
	{
		*result_listener << fmt::format(fg(fmt::color::red), "\nERROR:") <<
			fmt::format("\nMatrix has {} columns, but {} expected\n", arg.Cols(), rmatrix.Cols());
		return false;
	}

	constexpr const double DEFAULT_ACCURACY{1e-6};
	for(std::size_t row = 0;
		row < rmatrix.Rows(); row++)
	{
		for(std::size_t col = 0;
			col < rmatrix.Cols(); col++)
		{
			const auto difference{std::abs(arg(row, col) - rmatrix(row, col))};
			if(difference > DEFAULT_ACCURACY)
			{
				*result_listener << fmt::format(fg(fmt::color::red), "\nERROR:") <<
					fmt::format("\nGiven matrix value {} on {{{};{}}} "
							"is not nearly equals to {}.\n"
							"Difference is: {}\n",
							arg(row, col), row, col,
							rmatrix(row, col),
							difference);
				return false;
			}
		}
	}
	return testing::AssertionSuccess();
}

#endif // UNITTEST_COMMON_H
