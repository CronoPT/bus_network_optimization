#ifndef __PROBLEM_TPP__
#define __PROBLEM_TPP__ 

namespace genetic {

	template<typename T>
	problem<T>::problem(variable_set<T> variables):
	 _variables(variables) {

	}

	template<typename T>
	std::vector<float> problem<T>::compute_cost(const T& item) {
		auto costs = std::vector<float>();

		for (auto variable: _variables) {
			auto total = variable->objective(item);
			total += variable->transgression(item);
			costs.push_back(total);
		}

		return costs;
	}
	
	template<typename T>
	int problem<T>::num_variables() {
		return _variables.size();
	}

} // namespace genetic

#endif 