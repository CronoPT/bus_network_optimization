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
	class problem {

		private:
			constraint_set<T>    _constraints;
			cost_function_set<T> _cost_functions;

		public:
			problem(constraint_set<T> constraints, cost_function_set<T> cost_functions);
			std::vector<float>& compute_cost(const T& item);

	};

} // namespace genetic

#include "problem.tpp"

#endif