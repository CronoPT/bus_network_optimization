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

TEST_F(solution_test, solution_dominance_test) {
	auto solution_2 = genetic::solution<int>(20);
	auto solution_3 = genetic::solution<int>(30);
	auto solution_4 = genetic::solution<int>(40);
	auto solution_5 = genetic::solution<int>(50);
	auto solution_6 = genetic::solution<int>(60);
	auto solution_7 = genetic::solution<int>(60);
	

	_solution.costs({1, 1, 1});
	solution_2.costs({1, 2, 1});
	solution_3.costs({1, 1, 0.5});

	solution_4.costs({216.86, 144.51});
	solution_4.transgressions({0.0, 0.0});
	solution_4.total_transgression(0.0);
	solution_5.costs({218.16, 145.82});
	solution_5.transgressions({0.0, 0.0});
	solution_5.total_transgression(0.0);

	solution_6.costs({244.08, 145.61});
	solution_6.transgressions({0.0, 0.0});
	solution_6.total_transgression(0.0);
	solution_7.costs({245.38, 146.91});
	solution_7.transgressions({0.0, 0.0});
	solution_7.total_transgression(0.0);

	ASSERT_TRUE(_solution.dominates(solution_2));
	ASSERT_FALSE(_solution.dominates(solution_3));
	ASSERT_FALSE(solution_2.dominates(_solution));
	ASSERT_FALSE(solution_2.dominates(solution_3));
	ASSERT_TRUE(solution_3.dominates(_solution));
	ASSERT_TRUE(solution_3.dominates(solution_2));

	ASSERT_TRUE(solution_4.dominates(solution_5));
	ASSERT_TRUE(solution_4.is_feasible());
	ASSERT_TRUE(solution_4.constrained_dominates(solution_5));
	ASSERT_FALSE(solution_4.constrained_dominates(solution_4));	
	
	ASSERT_TRUE(solution_6.constrained_dominates(solution_7));
}
