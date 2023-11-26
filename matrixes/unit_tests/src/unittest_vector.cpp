#include "unittest_common.h"

#include "matrixes/vector.h"

using namespace MxLib;
using namespace MxLib::algo;

TEST(VectorConstructionTest, VectorDefaultConstructorTests)
{
	MxLib::VectorI vec{};
	ASSERT_EQ(vec.size(), 0);
}
TEST(VectorConstructionTest, VectorBasicConstructorTests)
{
	MxLib::VectorI vec{1, 2, 3, 4, 5};
	ASSERT_EQ(vec.size(), 5);

	const std::array expected{1, 2, 3, 4, 5};
	for (int i = 0; i < expected.size(); ++i)
	{
		ASSERT_EQ(vec[i], expected[i]);
	}
}
