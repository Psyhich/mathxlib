#include "unittest_common.h"

#include "matrixes/algorithms.h"
#include "matrixes/view.h"
#include "matrixes/minor.h"

using namespace MxLib;
using namespace MxLib::algo;


class MatrixConstructionTest :
	public ::testing::Test
{
};

TEST_F(MatrixConstructionTest, ColsRowsConstructionTestSuccessful_1)
{
	const MatrixD newMatrix = MatrixD{2, 3};
	ASSERT_EQ(newMatrix.Rows(), 2);
	ASSERT_EQ(newMatrix.Cols(), 3);
}
TEST_F(MatrixConstructionTest, ColsRowsConstructionTestSuccessful_2)
{
	const MatrixD newMatrix = MatrixD{0, 0};
	ASSERT_EQ(newMatrix.Rows(), 0);
	ASSERT_EQ(newMatrix.Cols(), 0);
}
TEST_F(MatrixConstructionTest, ColsRowsConstructionTestSuccessful_3)
{
	const MatrixD newMatrix = MatrixD{0, 5};
	ASSERT_EQ(newMatrix.Rows(), 0);
	ASSERT_EQ(newMatrix.Cols(), 5);
}
TEST_F(MatrixConstructionTest, SquareMatrixConstructionTestSuccessful_1)
{
	const MatrixD newMatrix = MatrixD{5};
	ASSERT_EQ(newMatrix.Rows(), 5);
	ASSERT_EQ(newMatrix.Cols(), 5);
}
TEST_F(MatrixConstructionTest, SquareMatrixConstructionTestSuccessful_2)
{
	const MatrixD newMatrix = MatrixD{0};
	ASSERT_EQ(newMatrix.Rows(), 0);
	ASSERT_EQ(newMatrix.Cols(), 0);
}
TEST_F(MatrixConstructionTest, SquareMatrixConstructionTestSuccessful_3)
{
	const MatrixD newMatrix = MatrixD{10000};
	ASSERT_EQ(newMatrix.Rows(), 10000);
	ASSERT_EQ(newMatrix.Cols(), 10000);
}
TEST_F(MatrixConstructionTest, InitializerListConstructionTestSuccessful_1)
{
	const MatrixD newMatrix{
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};
	ASSERT_EQ(newMatrix.Rows(), 3);
	ASSERT_EQ(newMatrix.Cols(), 3);
}
TEST_F(MatrixConstructionTest, InitializerListConstructionTestSuccessful_2)
{
	const MatrixD newMatrix{
		{ 1, 2, 3, 2 },
		{ 4, 5, 6, 2 },
		{ 7, 8, 9, 2 }
	};
	ASSERT_EQ(newMatrix.Rows(), 3);
	ASSERT_EQ(newMatrix.Cols(), 4);
}
TEST_F(MatrixConstructionTest, InitializerListConstructionTestSuccessful_3)
{
	const MatrixD newMatrix{{}};
	ASSERT_EQ(newMatrix.Rows(), 1);
	ASSERT_EQ(newMatrix.Cols(), 0);
}
TEST_F(MatrixConstructionTest, ByRankConstructionTestSuccessful)
{
	const MatrixD newMatrix{5};
	ASSERT_EQ(newMatrix.Rows(), 5);
	ASSERT_EQ(newMatrix.Cols(), 5);
}
TEST_F(MatrixConstructionTest, CopyConstructionTestSuccessful_1)
{
	const MatrixD newMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};
	const MatrixD copy{newMatrix};

	EXPECT_THAT(copy, IsEqualMatrix(newMatrix));
}
TEST_F(MatrixConstructionTest, CopyConstructionTestSuccessful_2)
{
	const MatrixD newMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};
	MatrixD copy{{1}};
	copy = newMatrix;

	EXPECT_THAT(copy, IsEqualMatrix(newMatrix));
}
TEST_F(MatrixConstructionTest, MoveConstructorTestSuccessful_1)
{
	MatrixD original{
		{ 1, 2 },
		{ 3, 4 }
	};

	const MatrixD moved{std::move(original)};

	ASSERT_EQ(original.Rows(), 0); // cppcheck-suppress[accessMoved]
	ASSERT_EQ(original.Cols(), 0); // cppcheck-suppress[accessMoved]

	ASSERT_EQ(moved.Rows(), 2);
	ASSERT_EQ(moved.Cols(), 2);
	
	const MatrixD expected{
		{ 1, 2 },
		{ 3, 4 }
	};
	EXPECT_THAT(moved, IsEqualMatrix(expected));
}
TEST_F(MatrixConstructionTest, MoveConstructorTestSuccessful_2)
{
	MatrixD original{
		{ 1, 2 },
		{ 3, 4 }
	};

	MatrixD moved{{ 1, 2 }};
	moved = std::move(original);

	ASSERT_EQ(original.Rows(), 0); // cppcheck-suppress[accessMoved]
	ASSERT_EQ(original.Cols(), 0); // cppcheck-suppress[accessMoved]

	ASSERT_EQ(moved.Rows(), 2);
	ASSERT_EQ(moved.Cols(), 2);
	
	const MatrixD expected{
		{ 1, 2 },
		{ 3, 4 }
	};
	EXPECT_THAT(moved, IsEqualMatrix(expected));
}
TEST(MatrixBoundCheckTest, MatrixInBoundGetterTestSuccessful)
{
	const MatrixD testMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};

	EXPECT_THAT(testMatrix(0, 1), testing::DoubleEq(2));
}
TEST(MatrixBoundCheckTest, MatrixOutOfBoundGetterTestUnsuccessful)
{
	const MatrixD testMatrix{
		{ 1, 2 },
		{ 3, 4 }
	};

	EXPECT_THROW({ testMatrix(0, 2); }, std::out_of_range);
}

TEST(MatrixViewTest, BasicConstructionTestSuccessful)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4 },
		{ -1, 3, 4 },
		{ 3, 6, 7 }
	};

	const MatrixD expected{
		{ 1, 3 },
		{ -1, 3 }
	};

	const MatrixView view{viewedMatrix, 0, 0, 2, 2};
	EXPECT_THAT(view, IsEqualMatrix(expected));
}
TEST(MatrixViewTest, BasicConstructionWithOutOfBoundsTestUnsuccessful)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4 },
		{ -1, 3, 4 },
		{ 3, 6, 7 }
	};

	const MatrixD expected{
		{ 1, 3 },
		{ -1, 3 }
	};

	EXPECT_THROW({(void)MatrixView(viewedMatrix, 2, 2, 2, 2);}, std::out_of_range);
}
TEST(MatrixViewTest, BasicConstructionWithOutOfBoundsSizeTestUnsuccessful)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4 },
		{ -1, 3, 4 },
		{ 3, 6, 7 }
	};

	const MatrixD expected{
		{ 1, 3 },
		{ -1, 3 }
	};

	EXPECT_THROW({(void)MatrixView(viewedMatrix, 0, 0, 5, 2);}, std::out_of_range);
}
TEST(MatrixViewTest, RecursiveViewTestSuccessful)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4, 2 },
		{ -1, 3, 4, 3 },
		{ 3, 6, 7, 4 }
	};

	const MatrixView view1{viewedMatrix, 0, 0, 3, 3};
	const MatrixView view2{view1, 1, 1, 2, 2};

	const MatrixD expected{
		{ 3, 4 },
		{ 6, 7 }
	};

	EXPECT_THAT(view2, IsEqualMatrix(expected));
}

TEST(MatrixMinorViewTest, BasicViewTestSuccessful_1)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4, 2 },
		{ -1, 3, 4, 3 },
		{ 3, 6, 7, 4 }
	};

	const MatrixD expected{
		{ 3, 4, 3 },
		{ 6, 7, 4 }
	};

	const MinorView minor{viewedMatrix, 0, 0};

	EXPECT_THAT(minor, IsEqualMatrix(expected));
}
TEST(MatrixMinorViewTest, BasicViewTestSuccessful_2)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4, 2 },
		{ -1, 3, 4, 3 },
		{ 3, 6, 7, 4 }
	};

	const MatrixD expected{
		{ 1, 3, 2 },
		{ 3, 6, 4 }
	};

	const MinorView minor{viewedMatrix, 1, 2};

	EXPECT_THAT(minor, IsEqualMatrix(expected));
}
TEST(MatrixMinorViewTest, OutOfBoundsViewTestUnsuccessful_1)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4, 2 },
		{ -1, 3, 4, 3 },
		{ 3, 6, 7, 4 }
	};

	EXPECT_THROW({ MinorView(viewedMatrix, 4, 5); }, std::out_of_range);
}
TEST(MatrixMinorViewTest, OutOfBoundsViewTestUnsuccessful_2)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4, 2 },
		{ -1, 3, 4, 3 },
		{ 3, 6, 7, 4 }
	};

	EXPECT_THROW({ MinorView(viewedMatrix, 0, 5); }, std::out_of_range);
}
TEST(MatrixMinorViewTest, OutOfBoundsViewTestUnsuccessful_3)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4, 2 },
		{ -1, 3, 4, 3 },
		{ 3, 6, 7, 4 }
	};

	EXPECT_THROW({ MinorView(viewedMatrix, 6, 0); }, std::out_of_range);
}
TEST(MatrixMinorViewTest, RecursiveViewTestSuccessful_1)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4, 2 },
		{ -1, 3, 4, 3 },
		{ 3, 6, 7, 4 }
	};

	const MinorView minor1{viewedMatrix, 0, 0};
	const MinorView minor2{minor1, 0, 0};

	const MatrixD expected{
		{ 7, 4 }
	};

	EXPECT_THAT(minor2, IsEqualMatrix(expected));
}
TEST(MatrixMinorViewTest, RecursiveViewTestSuccessful_2)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4, 2, 6, 42 },
		{ -1, 3, 4, 3, 8, 42 },
		{ 3, 6, 7, 4, 4, 42 },
		{ -1, 3, 4, 3, 8, 42 },
		{ 1, 3, 4, 2, 6, 42 }
	};

	const MinorView minor1{viewedMatrix, 0, 0};
	const MinorView minor2{minor1, 1, 1};
	const MinorView minor3{minor2, 2, 2};
	const MinorView minor4{minor3, 0, 0};

	const MatrixD expected{
		{ 3, 42 },
	};

	EXPECT_THAT(minor4, IsEqualMatrix(expected));
}
TEST(MatrixMinorViewTest, RecursiveViewWithOutOfBoundsTestUnsuccessful)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4, 2 },
		{ -1, 3, 4, 3 },
		{ 3, 6, 7, 4 }
	};

	const MinorView minor1{viewedMatrix, 0, 0};
	EXPECT_THROW({ MinorView(minor1, 5, 2); }, std::out_of_range);
}

TEST(MatrixRowViewTest, BasicViewTest)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4 },
		{ -1, 3, 4 },
		{ 3, 6, 7 }
	};

	const RowView rowView{viewedMatrix, 0};
	ASSERT_THAT(rowView.size(), viewedMatrix.Cols());

	for (std::size_t col = 0; col < rowView.size(); ++col)
	{
		ASSERT_THAT(rowView[col], viewedMatrix(0, col));
	}
}

TEST(MatrixColViewTest, BasicViewTest)
{
	const MatrixD viewedMatrix{
		{ 1, 3, 4 },
		{ -1, 3, 4 },
		{ 3, 6, 7 }
	};

	const ColView rowView{viewedMatrix, 0};
	ASSERT_THAT(rowView.size(), viewedMatrix.Rows());

	for (std::size_t row = 0; row < rowView.size(); ++row)
	{
		ASSERT_THAT(rowView[row], viewedMatrix(row, 0));
	}
}
