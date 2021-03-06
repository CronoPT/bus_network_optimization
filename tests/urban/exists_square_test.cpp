#include <gtest/gtest.h>
#include <grid.hpp>
#include <configs.hpp>

TEST(exists_square_test, exists_square_simple_test) {
	if (urban::configs::odx_matrix_data == "../data/json/odx_matrix.json") {
		ASSERT_FALSE(urban::grid::instance()->exists_square(0, 0));
		urban::grid::instance()->get_squares()[0][0] = urban::square(0, 0);
		ASSERT_TRUE(urban::grid::instance()->exists_square(0, 0));
		ASSERT_FALSE(urban::grid::instance()->exists_square(0, 1));
		urban::grid::instance()->get_squares()[0][1] = urban::square(0, 1);
		ASSERT_TRUE(urban::grid::instance()->exists_square(0, 1));
		urban::grid::instance()->get_squares()[0].erase(0);
		urban::grid::instance()->get_squares()[0].erase(1);
	}
}