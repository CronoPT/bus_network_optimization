#include <gtest/gtest.h>
#include <odx_matrix.hpp>

TEST(odx_matrix_test, odx_matrix_simple_test) {

	auto origin = std::pair<int, int>({3, 10});
	auto destin = std::pair<int, int>({5, 9});

	ASSERT_EQ(29, urban::odx_matrix::instance()->get_morning_rush_hour(origin, destin));
	ASSERT_EQ(29, urban::odx_matrix::instance()->get_midday(origin, destin));
	ASSERT_EQ(11, urban::odx_matrix::instance()->get_afternoon_rush_hour(origin, destin));
	ASSERT_EQ(6,  urban::odx_matrix::instance()->get_happy_hour(origin, destin));
	ASSERT_EQ(0,  urban::odx_matrix::instance()->get_night_time(origin, destin));
	ASSERT_EQ(75, urban::odx_matrix::instance()->get_total(origin, destin));

	origin = std::pair<int, int>({26, 23});
	destin = std::pair<int, int>({23, 21});	

	ASSERT_EQ(4, urban::odx_matrix::instance()->get_morning_rush_hour(origin, destin));
	ASSERT_EQ(7, urban::odx_matrix::instance()->get_midday(origin, destin));
	ASSERT_EQ(0, urban::odx_matrix::instance()->get_afternoon_rush_hour(origin, destin));
	ASSERT_EQ(0,  urban::odx_matrix::instance()->get_happy_hour(origin, destin));
	ASSERT_EQ(1,  urban::odx_matrix::instance()->get_night_time(origin, destin));
	ASSERT_EQ(12, urban::odx_matrix::instance()->get_total(origin, destin));

}