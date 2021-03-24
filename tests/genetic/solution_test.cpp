#include <solution.hpp>
#include <gtest/gtest.h>

class solution_test: public testing::Test {  

	protected:
		genetic::solution<int> _solution = genetic::solution<int>(10);

};

TEST_F(solution_test, solution_constructor_test) {
	ASSERT_FLOAT_EQ(_solution.total_cost(), -1);
	ASSERT_FLOAT_EQ(_solution.fitness(),    -1);
	ASSERT_EQ(_solution.item(), 10);
	ASSERT_EQ(_solution.costs(), std::vector<float>());
}

TEST_F(solution_test, solution_setters_test) {	
	_solution.total_cost(60);
	_solution.fitness(0.5);
	_solution.item(90);
	_solution.costs({10, 20, 30});
	
	ASSERT_FLOAT_EQ(_solution.total_cost(), 60);
	ASSERT_FLOAT_EQ(_solution.fitness(),    0.5);
	ASSERT_EQ(_solution.item(), 90);
	ASSERT_EQ(_solution.costs(), std::vector<float>({10, 20, 30}));
}

TEST_F(solution_test, solution_comparison_test) {
	genetic::solution<int> other_solution = genetic::solution<int>(20);
	
	_solution.fitness(0.5);
	_solution.total_cost(0.5);
	other_solution.fitness(0.7);
	other_solution.total_cost(0.7);

	ASSERT_TRUE(_solution < other_solution);
}

