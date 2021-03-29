#ifndef __PROBLEM_HPP__
#define __PROBLEM_HPP__ 

#include "variable.hpp"
#include <memory>
#include <vector>

namespace genetic {

	template<typename T>
	using variable_set = std::vector<std::shared_ptr<variable<T>>>; 

	template<typename T>
	class problem {

		private:
			variable_set<T> _variables;

		public:
			problem(variable_set<T> variables);
			std::vector<float> compute_cost(const T& item);
			virtual std::vector<solution<T>> initialize_population() = 0;
			virtual std::pair<T, T> crossover(T& i1, T& i2) = 0;
			virtual void mutate(T& item) const = 0;
			int num_variables();

	};

} // namespace genetic

#include "problem.tpp"

#endif