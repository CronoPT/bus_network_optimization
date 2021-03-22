#include <genetic_solution.hpp>
#include <gtest/gtest.h>

class numeric_solution: public genetic::genetic_solution<int> {

	public:
		numeric_solution(int item): genetic::genetic_solution<int>(item) {

		}

		void dump_to(std::ostream& os) const override {
			os << "Solution: " << item();
		}

}; 

class genetic_solution_test: public testing::Test {  

	protected:
		numeric_solution _solution = numeric_solution(10);

};

TEST_F(genetic_solution_test, genetic_solution_constructor_test) {
	ASSERT_FLOAT_EQ(_solution.total_cost(), -1);
	ASSERT_FLOAT_EQ(_solution.fitness(),    -1);
	ASSERT_EQ(_solution.item(), 10);
	ASSERT_EQ(_solution.costs(), std::vector<float>());
}
