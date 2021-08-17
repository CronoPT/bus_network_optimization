#ifndef __POPULATION_TPP__
#define __POPULATION_TPP__ 

namespace genetic {

	template<typename T>
	population<T>::population():
	 _solutions() {

	}

	template<typename T>
	population<T>::population(std::vector<solution<T>>  sols):
	 _solutions(sols),
	 _original_size(sols.size()) {
		 std::sort(_solutions.begin(), _solutions.end());
	}

	template<typename T>
	void population<T>::add_solution(solution<T>& sol) {
	 	_solutions.push_back(sol);
		std::sort(_solutions.begin(), _solutions.end());
	}

	template<typename T>
	void population<T>::add_solution(T& sol) {
		_solutions.push_back(solution<T>(sol));
		std::sort(_solutions.begin(), _solutions.end());
	}

	template<typename T>
	void population<T>::clip() {
		int new_size = std::ceil((float)_solutions.size()/2);
		_solutions.resize(_original_size);
	}

	template<typename T>
	void population<T>::clip_back() {
		std::reverse(_solutions.begin(), _solutions.end());
		int new_size = std::ceil((float)_solutions.size()/2);
		_solutions.resize(new_size);
	}

	template<typename T>
	solution<T>& population<T>::get_nth_best(int n) {
		std::sort(_solutions.begin(), _solutions.end());
		return _solutions.at(n);
	}

	template<typename T>
	std::vector<solution<T>>& population<T>::get_solutions() {
		return _solutions;
	}

	template<typename T>
	void population<T>::reset_ranks() {
		for (auto& solution: _solutions) {
			solution.reset_rank();
		}
	}

	template<typename T>
	int population<T>::size() {
		return _solutions.size();
	}

	template<typename T>
	void population<T>::reset_crowding_distance() {
		for (auto& solution: _solutions) {
			solution.reset_crowding_distance();
		}
	}

} // namespace genetic

#endif