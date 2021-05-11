#ifndef __PROBLEM_TPP__
#define __PROBLEM_TPP__ 

namespace genetic {

	template<typename T>
	problem<T>::problem(constraint_set<T> constraints, cost_function_set<T> cost_functions):
	 _constraints(constraints), _cost_functions(cost_functions) {

	}

	template<typename T>
	evaluation_report<T> problem<T>::compute_cost(T& item) {
		auto costs = std::vector<float>();
		auto trans = std::vector<float>();

		for (auto constraint: _constraints) {
			trans.push_back( constraint->transgression(item) );
		}

		for (auto cost_function: _cost_functions) {
			costs.push_back( cost_function->compute(item) );
		}

		return evaluation_report<T>(costs, trans);
	}

	template<typename T>
	int problem<T>::num_constraints() {
		return _constraints.size();
	}

	template<typename T>
	int problem<T>::num_cost_functions() {
		return _cost_functions.size();
	}

} // namespace genetic

#endif 