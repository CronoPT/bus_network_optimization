#ifndef __POPULATION_TPP__
#define __POPULATION_TPP__ 

namespace genetic {

	template<typename T>
	population<T>::population():
	 _solutions() {

	}

	template<typename T>
	population<T>::population(const solution_set<T>& sols):
	 _solutions(sols) {
		 std::sort(_solutions.begin(), _solutions.end(), std::greater<solution<T>>());
	}

	template<typename T>
	void population<T>::add_solution(const solution<T>& sol) {
	 	_solutions.push_back(sol);
		std::sort(_solutions.begin(), _solutions.end(), std::greater<solution<T>>());
	}

	template<typename T>
	void population<T>::add_solution(const T& sol) {
		_solutions.push_back(solution<T>(sol));
		std::sort(_solutions.begin(), _solutions.end(), std::greater<solution<T>>());
	}

	template<typename T>
	void population<T>::clip() {
		std::sort(_solutions.begin(), _solutions.end(), std::greater<solution<T>>());
		int new_size = std::ceil((float)_solutions.size()/2);
		_solutions.resize(new_size);
	}

	template<typename T>
	solution<T>& population<T>::nth_best(int n) {
		std::sort(_solutions.begin(), _solutions.end(), std::greater<solution<T>>());
		return _solutions.at(0);
	}

	template<typename T>
	solution_set<T>& population<T>::solutions() {
		return _solutions;
	}

} // namespace genetic

#endif