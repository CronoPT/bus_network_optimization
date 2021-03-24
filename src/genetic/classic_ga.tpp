#ifndef __CLASSIC_GA_TPP__
#define __CLASSIC_GA_TPP__

namespace genetic {

	template<typename T>
	classic_ga<T>::classic_ga(problem<T>* problem):
	 algorithm<T>(problem) {

	}

	template<typename T>
	solution<T> classic_ga<T>::execute(
		int max_iterations, 
		float min_improv
	) {
		this->initialize_population();

		int iteration = 0;
		float cost_diff = min_improv+0.1;

		while (iteration<max_iterations && cost_diff>min_improv) {
			float best_cost = this->best_solution().total_cost();

			this->print_population();

			this->assign_fitness();
			this->reproduce();

			this->print_population();

			float new_best = this->best_solution().total_cost();
			cost_diff = (best_cost-new_best)/best_cost;
			iteration += 1;
		}

		std::cout << iteration << std::endl;
		std::cout << cost_diff << std::endl;

		return this->best_solution();
	}

	template<typename T>
	void classic_ga<T>::iteration() const {
		//TODO: reflect on the usefulness of this abstraction
	}

} // namespace genetic

#endif