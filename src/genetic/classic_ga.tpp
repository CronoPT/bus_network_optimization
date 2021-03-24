#ifndef __CLASSIC_GA_TPP__
#define __CLASSIC_GA_TPP__

namespace genetic {

	template<typename T>
	classic_ga<T>::classic_ga(problem<T>* problem):
	 algorithm(problem) {

	}

	template<typename T>
	solution<T> classic_ga<T>::execute(
		int max_iterations, 
		float min_improv
	) {
		initialize_population();

		int iteration = 0;
		float cost_diff = min_improv+0.1;

		while (iteration<max_iterations && cost_diff>min_improv) {
			float best_cost = _population.nth_best(0).total_cost();

			assign_fitness();
			reproduce();

			float new_best = _population.nth_best(0).total_cost();
			cost_diff = (best_cost-new_best)/best_cost;
			iterations += 1;
		}

		return _population.nth_best(0);
	}

	template<typename T>
	void classic_ga<T>::iteration() {
		//TODO: reflect on the usefulness of this abstraction
	}

} // namespace genetic

#include "classic_ga.tpp"

#endif