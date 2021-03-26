#ifndef __SOLUTION_TPP__
#define __SOLUTION_TPP__ 


namespace genetic {

	template<typename T>
	solution<T>::solution(const T& item):
	 _total_cost(-1), _fitness(-1), _costs(0, 0), _item(item),
	 _rank(-1), _domination_count(0), _dominates(),
	 _crowding_distance(0) {

	}

	template<typename T>
	solution<T>::solution():
	 _total_cost(-1), _fitness(-1), _costs(0, 0), _item(),
	 _rank(-1), _domination_count(0), _dominates(),
	 _crowding_distance(0) {

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
	int solution<T>::rank() const {
		return _rank;
	}

	template<typename T>
	int solution<T>::domination_count() const {
		return _domination_count;
	}
	
	template<typename T>
	std::vector<int>& solution<T>::dominates() {
		return _dominates;
	}

	template<typename T>
	float solution<T>::crowding_distance() {
		return _crowding_distance;
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
	void solution<T>::rank(int new_rank) {
		_rank = new_rank;
	}

	template<typename T>
	void solution<T>::domination_count(int new_count) {
		_domination_count = new_count;
	}

	template<typename T>
	void solution<T>::dominates(std::vector<int>& new_dominates) {
		_dominates = new_dominates;
	}

	template<typename T>
	void solution<T>::crowding_distance(float new_distance) {
		_crowding_distance = new_distance;
	}

	template<typename T>
	bool solution<T>::dominates(solution<T>& other) {
		bool least_as_good = true;
		bool better_in_one = false;

		for (int i=0; i<_costs.size(); i++) {
			least_as_good &= _costs.at(i) <= other.costs().at(i);
			better_in_one |= _costs.at(i) <  other.costs().at(i);
		}

		return least_as_good && better_in_one;
	}

	template<typename T>
	void solution<T>::inc_domination_count() {
		++_domination_count;
	}

	template<typename T>
	void solution<T>::dec_domination_count() {
		--_domination_count;
	}

	template<typename T>
	void solution<T>::add_dominates(int dominated) {
		_dominates.push_back(dominated);
	}

	template<typename T>
	void solution<T>::rem_dominates(int dominated) {
		_dominates.erase(std::remove(_dominates.begin(), _dominates.end(), dominated));
	}

	template<typename T>
	void solution<T>::reset_rank() {
		_rank = -1;	
	}

	template<typename T>
	bool solution<T>::has_rank() {
		return _rank!=-1;
	}

	template<typename T>
	bool solution<T>::dominates(int other) {
		return (std::find(_dominates.begin(), _dominates.end(), other) 
		        != _dominates.end());
	}

	template<typename T>
	void solution<T>::reset_crowding_distance() {
		_crowding_distance = 0;
	}

	template<typename T>
	void solution<T>::add_crowding_distance(float increment) {
		_crowding_distance += increment;
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
		   << "\n\tfitness: " << s._fitness 
		   << "\n\trank: " << s._rank << "\n\tcosts: ";
		
		for (auto cost: s._costs)
			os << cost << " ";

		os << "\n\tdomination count: " << s._domination_count
		   << "\n\tdominates: ";

		for (auto dominated: s._dominates)
			os << dominated << " ";

		os << "\n\tcrowding distance: " << s._crowding_distance;

		return os;
	}

} // namespace genetic

#endif
