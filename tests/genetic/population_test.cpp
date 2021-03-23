#include <population.hpp>
#include <solution.hpp>
#include <gtest/gtest.h>

class population_test: public testing::Test {  

	protected:
		genetic::population<int> _population = genetic::population<int>();

};

TEST_F(population_test, population_constructor_test) {
	genetic::solution<int> sol1 = genetic::solution<int>(80);
	sol1.fitness(0.8);
	_population.add_solution(sol1);
	genetic::solution<int> sol2 = genetic::solution<int>(70);
	sol2.fitness(0.7);
	_population.add_solution(sol2);
	genetic::solution<int> sol3 = genetic::solution<int>(50);
	sol3.fitness(0.5);
	_population.add_solution(sol3);
	genetic::solution<int> sol4 = genetic::solution<int>(60);
	sol4.fitness(0.6);
	_population.add_solution(sol4);

	ASSERT_EQ(sol1, _population.nth_best(0));
	ASSERT_EQ(sol2, _population.nth_best(1));
	ASSERT_EQ(sol3, _population.nth_best(3));
	ASSERT_EQ(sol4, _population.nth_best(2));
}