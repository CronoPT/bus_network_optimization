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
	const std::vector<float>& solution<T>::costs() const {
		return _costs;
	}

	template<typename T>
	const T& solution<T>::item() const {
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
	bool solution<T>::operator<(const solution<T>& other) const {
		return _fitness < other.fitness();
	}

	template<typename T>
	bool solution<T>::operator>(const solution<T>& other) const {
		return _fitness > other.fitness();
	}

	template<typename T>
	bool solution<T>::operator==(const solution<T>& other) const {
		return _item == other.item();
	}

	template<typename T>
	void solution<T>::dump_to(std::ostream& os) const {
		os << "Solution: " << item();
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const genetic::solution<T>& s) {
		s.dump_to(os);
		return os;
	}

} // namespace genetic

#endif
