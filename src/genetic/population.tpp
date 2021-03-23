#ifndef __POPULATION_TPP__
#define __POPULATION_TPP__ 

namespace genetic {

	template<typename T>
	population<T>::population():
	 _solutions(){

	}

	template<typename T>
	population<T>::population(std::vector<solution<T>>& sols):
	_solutions(){
		for(auto sol : sols) {
			_solutions.insert(sol);
		}
	}

	template<typename T>
	void population<T>::compute_costs() {
		//TODO: Implement this function
	}

	template<typename T>
	void population<T>::assign_fitness() {
		//TODO: Implement this function
	}

	template<typename T>
	void population<T>::add_solution(const solution<T>& sol) {
		_solutions.insert(sol);
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

} // namespace genetic

#endif