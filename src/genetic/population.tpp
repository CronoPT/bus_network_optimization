#ifndef __POPULATION_TPP__
#define __POPULATION_TPP__ 

namespace genetic {

	template<typename T>
	population<T>::population():
	 _solutions() {

	}

	template<typename T>
	population<T>::population(std::vector<solution<T>>  sols):
	 _solutions(sols) {
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
		std::sort(_solutions.begin(), _solutions.end());
		int new_size = std::ceil((float)_solutions.size()/2);
		_solutions.resize(new_size);
	}

	template<typename T>
	solution<T>& population<T>::nth_best(int n) {
		std::sort(_solutions.begin(), _solutions.end());
		return _solutions.at(n);
	}

	template<typename T>
	std::vector<solution<T>>& population<T>::solutions() {
		return _solutions;
	}

} // namespace genetic

#endif