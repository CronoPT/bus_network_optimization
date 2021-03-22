#include <solution.hpp>
#include <gtest/gtest.h>

class numeric_solution: public genetic::solution<int> {

	public:
		numeric_solution(int item): genetic::solution<int>(item) {

		}

		void dump_to(std::ostream& os) const override {
			os << "Solution: " << item();
		}

}; 

class solution_test: public testing::Test {  

	protected:
		numeric_solution _solution = numeric_solution(10);

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
	numeric_solution other_solution = numeric_solution(20);
	
	_solution.fitness(0.5);
	other_solution.fitness(0.7);

	ASSERT_TRUE(_solution < other_solution);
}

