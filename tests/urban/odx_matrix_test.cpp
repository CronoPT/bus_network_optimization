#include <gtest/gtest.h>
#include <odx_matrix.hpp>

class odx_matrix_test: public testing::Test {  

	protected:
		urban::odx_matrix _matrix = urban::odx_matrix();

};

TEST_F(odx_matrix_test, odx_matrix_simple_test) {

	auto origin = std::pair<int, int>({3, 10});
	auto destin = std::pair<int, int>({5, 9});

	ASSERT_EQ(29, _matrix.get_morning_rush_hour(origin, destin));
	ASSERT_EQ(29, _matrix.get_midday(origin, destin));
	ASSERT_EQ(11, _matrix.get_afternoon_rush_hour(origin, destin));
	ASSERT_EQ(6,  _matrix.get_happy_hour(origin, destin));
	ASSERT_EQ(0,  _matrix.get_night_time(origin, destin));
	ASSERT_EQ(75, _matrix.get_total(origin, destin));

	origin = std::pair<int, int>({26, 23});
	destin = std::pair<int, int>({23, 21});	

	ASSERT_EQ(4, _matrix.get_morning_rush_hour(origin, destin));
	ASSERT_EQ(7, _matrix.get_midday(origin, destin));
	ASSERT_EQ(0, _matrix.get_afternoon_rush_hour(origin, destin));
	ASSERT_EQ(0,  _matrix.get_happy_hour(origin, destin));
	ASSERT_EQ(1,  _matrix.get_night_time(origin, destin));
	ASSERT_EQ(12, _matrix.get_total(origin, destin));

}