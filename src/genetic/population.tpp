#ifndef __POPULATION_TPP__
#define __POPULATION_TPP__ 

namespace genetic {

	template<typename T>
	population<T>::population():
	 _solutions() {

	}

	template<typename T>
	population<T>::population(solution_set<T>& sols):
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
	solution<T> population<T>::nth_best(int n) const {
		if (_solutions.size() < n) {
			throw "Population does not have that many elements";
		}

		auto it = _solutions.begin();
		std::advance(it, n);
		return *it;
	}

	template<typename T>
	solution_set<T> population<T>::solutions() const {
		return _solutions;
	}

} // namespace genetic

#endif