#include <gtest/gtest.h>
#include <grid.hpp>

class exists_square_test: public testing::Test {  

	protected:
		urban::grid _grid = urban::grid();

};

TEST_F(exists_square_test, exists_square_simple_test) {

	ASSERT_FALSE(_grid.exists_square(0, 0));
	_grid.get_squares()[0][0] = urban::square(0, 0);
	ASSERT_TRUE(_grid.exists_square(0, 0));
	ASSERT_FALSE(_grid.exists_square(0, 1));
	_grid.get_squares()[0][1] = urban::square(0, 1);
	ASSERT_TRUE(_grid.exists_square(0, 1));

}