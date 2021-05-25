#ifndef __PROBLEM_HPP__
#define __PROBLEM_HPP__ 

#include "constraint.hpp"
#include "cost_function.hpp"
#include <memory>
#include <vector>

namespace genetic {

	template<typename T>
	using constraint_set = std::vector<std::shared_ptr<constraint<T>>>; 

	template<typename T>
	using cost_function_set = std::vector<std::shared_ptr<cost_function<T>>>; 

	template<typename T>
	using evaluation_report = std::pair<std::vector<float>, std::vector<float>>; 

	template<typename T>
	class problem {

		private:
			constraint_set<T>    _constraints;
			cost_function_set<T> _cost_functions;

		public:
			problem(constraint_set<T> constraints, cost_function_set<T> cost_functions);
			evaluation_report<T> compute_cost(T& item);
			virtual std::vector<solution<T>> initialize_population(int pop_size) = 0;
			virtual std::pair<T, T> crossover(T& i1, T& i2) = 0;
			virtual void mutate(T& item) const = 0;
			int num_constraints();
			int num_cost_functions();

	};

} // namespace genetic

#include "problem.tpp"

#endif