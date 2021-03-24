#include <population.hpp>
#include <solution.hpp>
#include <gtest/gtest.h>

class population_test: public testing::Test {  

	protected:
		genetic::population<int> _population = genetic::population<int>();

};

TEST_F(population_test, population_empty_constructor_test) {
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

TEST_F(population_test, population_vector_constructor_test) {
	auto solutions = std::vector<genetic::solution<int>>();

	genetic::solution<int> sol1 = genetic::solution<int>(80);
	sol1.fitness(0.8);
	solutions.push_back(sol1);
	genetic::solution<int> sol2 = genetic::solution<int>(70);
	sol2.fitness(0.7);
	solutions.push_back(sol2);
	genetic::solution<int> sol3 = genetic::solution<int>(50);
	sol3.fitness(0.5);
	solutions.push_back(sol3);
	genetic::solution<int> sol4 = genetic::solution<int>(60);
	sol4.fitness(0.6);
	solutions.push_back(sol4);

	_population = genetic::population<int>(solutions);

	ASSERT_EQ(sol1, _population.nth_best(0));
	ASSERT_EQ(sol2, _population.nth_best(1));
	ASSERT_EQ(sol3, _population.nth_best(3));
	ASSERT_EQ(sol4, _population.nth_best(2));
}

TEST_F(population_test, population_clip_even_test) {
	auto solutions = std::vector<genetic::solution<int>>();

	genetic::solution<int> sol1 = genetic::solution<int>(80);
	sol1.fitness(0.8);
	solutions.push_back(sol1);
	genetic::solution<int> sol2 = genetic::solution<int>(70);
	sol2.fitness(0.7);
	solutions.push_back(sol2);
	genetic::solution<int> sol3 = genetic::solution<int>(50);
	sol3.fitness(0.5);
	solutions.push_back(sol3);
	genetic::solution<int> sol4 = genetic::solution<int>(60);
	sol4.fitness(0.6);
	solutions.push_back(sol4);

	_population = genetic::population<int>(solutions);

	ASSERT_EQ(sol1, _population.nth_best(0));
	ASSERT_EQ(sol2, _population.nth_best(1));
	ASSERT_EQ(sol3, _population.nth_best(3));
	ASSERT_EQ(sol4, _population.nth_best(2));
	ASSERT_EQ(4,    _population.solutions().size());

	_population.clip();

	ASSERT_EQ(sol1, _population.nth_best(0));
	ASSERT_EQ(sol2, _population.nth_best(1));
	ASSERT_EQ(2,    _population.solutions().size());
}

TEST_F(population_test, population_clip_odd_test) {
	auto solutions = std::vector<genetic::solution<int>>();

	genetic::solution<int> sol1 = genetic::solution<int>(80);
	sol1.fitness(0.8);
	solutions.push_back(sol1);
	genetic::solution<int> sol2 = genetic::solution<int>(70);
	sol2.fitness(0.7);
	solutions.push_back(sol2);
	genetic::solution<int> sol3 = genetic::solution<int>(50);
	sol3.fitness(0.5);
	solutions.push_back(sol3);

	_population = genetic::population<int>(solutions);

	ASSERT_EQ(sol1, _population.nth_best(0));
	ASSERT_EQ(sol2, _population.nth_best(1));
	ASSERT_EQ(sol3, _population.nth_best(2));
	ASSERT_EQ(3,    _population.solutions().size());

	_population.clip();

	ASSERT_EQ(sol1, _population.nth_best(0));
	ASSERT_EQ(sol2, _population.nth_best(1));
	ASSERT_EQ(2,    _population.solutions().size());
}