#ifndef __SOLUTION_TPP__
#define __SOLUTION_TPP__ 


namespace genetic {

	template<typename T>
	solution<T>::solution(const T& item):
	 _total_cost(-1), _fitness(-1), _costs(0, 0), _item(item) {

	}

	template<typename T>
	solution<T>::solution():
	 _total_cost(-1), _fitness(-1), _costs(0, 0), _item() {

	}


	template<typename T>
	float solution<T>::total_cost() const {
		return _total_cost;
	}

	template<typename T>
	float solution<T>::fitness() const {
		return _fitness;
	}

	template<typename T>
	std::vector<float>& solution<T>::costs() {
		return _costs;
	}

	template<typename T>
	T& solution<T>::item() {
		return _item;
	}

	template<typename T>
	void solution<T>::total_cost(float new_total) {
		_total_cost = new_total;
	}

	template<typename T>
	void solution<T>::fitness(float new_fitness) {
		_fitness = new_fitness;
	}

	template<typename T>
	void solution<T>::costs(const std::vector<float>& new_costs) {
		_costs = new_costs;
	}

	template<typename T>
	void solution<T>::item(const T& new_item) {
		_item = new_item;
	}

	template<typename T>
	bool operator<(const solution<T>& s1, const solution<T>& s2) {
		return s1._total_cost < s2._total_cost;
	}

	template<typename T>
	bool operator>(const solution<T>& s1, const solution<T>& s2) {
		return s1._total_cost > s2._total_cost;
	}

	template<typename T>
	bool operator==(const solution<T>& s1, const solution<T>& s2) {
		return s1._item == s2._item;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const solution<T>& s) {
		os << "Solution: \n\titem: " << s._item
		   << "\n\tcost: " << s._total_cost
		   << "\n\tfitness: " << s._fitness << "\n\tcosts: ";
		
		for (auto cost: s._costs)
			os << cost << " ";

		return os;
	}

} // namespace genetic

#endif
