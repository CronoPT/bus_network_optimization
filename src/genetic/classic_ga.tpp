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
		float min_improv,
		int max_stalled
	) {
		this->initialize_population();
		this->compute_costs();

		int iteration = 0;
		float cost_diff = min_improv+0.1;
		int insig_iters = 0;

		std::cout << "Max Stalled: " << max_stalled
		          << "\tInsignificant Iters: " << insig_iters << std::endl;

		while (iteration<max_iterations && insig_iters<max_stalled) {
			std::cout << "\n<<<<<<<<NEW POPULATION>>>>>>>>" << std::endl;

			float best_cost = this->best_solution().total_cost();
			this->assign_fitness();
			this->print_population();
			this->reproduce();
			this->mutate();
			this->compute_costs();
			this->clip_population();
			float new_best = this->best_solution().total_cost();

			cost_diff = (best_cost-new_best)/best_cost;
			if (cost_diff < min_improv) { ++insig_iters; } 
			else { insig_iters = 0; }

			iteration += 1;
		}

		std::cout << "\nClassic GA is finished!" << std::endl;
		std::cout << "Iterations taken: " << iteration << std::endl;
		std::cout << "Cost diff from previous iteration: " << cost_diff
		          << "\n" << std::endl;

		return this->best_solution();
	}

	template<typename T>
	void classic_ga<T>::iteration() const {
		//TODO: reflect on the usefulness of this abstraction
	}

} // namespace genetic

#endif