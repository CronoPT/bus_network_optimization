#ifndef __ALGORITHM_TPP__
#define __ALGORITHM_TPP__ 

namespace genetic {

	template<typename T>
	bool algorithm<T>::do_crossover() {
		float rand_nu = ((float) std::rand() / RAND_MAX);
		return genetic_configs::crossover_probability >= rand_nu;
	}

	template<typename T>
	bool algorithm<T>::do_mutate() {
		float rand_nu = ((float) std::rand() / RAND_MAX);
		return genetic_configs::mutation_probability >= rand_nu;
	}

	template<typename T>
	int algorithm<T>::select_solution() {
		float rand_nu = ((float) std::rand() / RAND_MAX);

		float sum = 0.0;
		for (int i=0; i<_population.get_solutions().size(); i++) {
			sum += _population.get_solutions().at(i).get_fitness();
			if (sum >= rand_nu) { return i; }
		}

		return 0;
	}

	template<typename T>
	algorithm<T>::algorithm(problem<T>* problem):
	 _population(), _problem(problem) {

	}

	template<typename T>
	void algorithm<T>::initialize_population() {
		_population = population<T>(
			_problem->initialize_population(
				genetic_configs::population_size
			)
		);
		compute_costs();
	}

	template<typename T>
	void algorithm<T>::reproduce() {
		auto crossovers = std::vector<std::pair<int, int>>();
		for (int i=0; i<_population.get_solutions().size()/2; i++) {
			int sol1 = select_solution();
			int sol2 = select_solution();
			crossovers.push_back(std::pair<int, int>(sol1, sol2));
		}

		for (auto pair: crossovers) {
			auto new_pair = crossover(
				_population.get_solutions().at(pair.first).get_item(),
				_population.get_solutions().at(pair.second).get_item()
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
		for (auto& solution: _population.get_solutions()) {
			if (do_mutate())
				_problem->mutate(solution.get_item());
		}
	}

	template<typename T>
	void algorithm<T>::clip_population() {
		_population.clip();
	}

	template<typename T>
	void algorithm<T>::clip_population_back() {
		_population.clip_back();
	}

	template<typename T>
	solution<T> algorithm<T>::get_best_solution() {
		return _population.get_nth_best(0);
	}

	template<typename T>
	void algorithm<T>::print_population() {
		std::cout << "OOOOOooooo<<<<<<<<<<<<<<<<<<<<< Population size: " << _population.size()
		          << " >>>>>>>>>>>>>>>>>>>>>oooooOOOOO" << std::endl;
		for (auto solution: _population.get_solutions()) {
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