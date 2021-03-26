#ifndef __ALGORITHM_TPP__
#define __ALGORITHM_TPP__ 

namespace genetic {

	template<typename T>
	bool algorithm<T>::do_crossover() {
		float rand_nu = ((float) std::rand() / RAND_MAX);
		return _crossover_prob >= rand_nu;
	}

	template<typename T>
	bool algorithm<T>::do_mutate() {
		float rand_nu = ((float) std::rand() / RAND_MAX);
		return _mutation_prob >= rand_nu;
	}

	template<typename T>
	int algorithm<T>::select_solution() {
		float rand_nu = ((float) std::rand() / RAND_MAX);

		float sum = 0.0;
		for (int i=0; i<_population.solutions().size(); i++) {
			sum += _population.solutions().at(i).fitness();
			if (sum >= rand_nu) { return i; }
		}

		return 0;
	}

	template<typename T>
	algorithm<T>::algorithm(problem<T>* problem):
	 _population(), _problem(problem), 
	 _crossover_prob(0.8), _mutation_prob(0.8) {

	}

	template<typename T>
	void algorithm<T>::initialize_population() {
		_population = population<T>(
			_problem->initialize_population()
		);
		compute_costs();
	}

	template<typename T>
	void algorithm<T>::reproduce() {
		auto crossovers = std::vector<std::pair<int, int>>();
		for (int i=0; i<_population.solutions().size()/2; i++) {
			int sol1 = select_solution();
			int sol2 = select_solution();
			crossovers.push_back(std::pair<int, int>(sol1, sol2));
		}

		for (auto pair: crossovers) {
			auto new_pair = crossover(
				_population.solutions().at(pair.first).item(),
				_population.solutions().at(pair.second).item()
			);
			_population.add_solution(new_pair.first);
			_population.add_solution(new_pair.second);
		}

	}

	template<typename T>
	std::pair<T, T> algorithm<T>::crossover(T i1, T i2) {
		if (do_crossover()) {
			return _problem->crossover(i1, i2);
		} else {
			return std::pair<T, T>(i1, i2);
		}
	}

	template<typename T>
	void algorithm<T>::mutate() {
		for (auto& solution: _population.solutions()) {
			if (do_mutate())
				_problem->mutate(solution.item());
		}
	}

	template<typename T>
	void algorithm<T>::clip_population() {
		_population.clip();
	}

	template<typename T>
	solution<T> algorithm<T>::best_solution() {
		return _population.nth_best(0);
	}

	template<typename T>
	void algorithm<T>::print_population() {
		std::cout << "OOOOOooooo<<<<<<<<<<<<<<<<<<<<< Population size: " << _population.size()
		          << " >>>>>>>>>>>>>>>>>>>>>oooooOOOOO" << std::endl;
		for (auto solution: _population.solutions()) {
			std::cout << solution << std::endl;
		}
	}

	template<typename T>
	population<T>& algorithm<T>::get_population() {
		return _population;
	}

	template<typename T>
	problem<T>* algorithm<T>::get_problem() {
		return _problem;
	}

} // namespace genetic

#endif