#ifndef __CLASSIC_GA_TPP__
#define __CLASSIC_GA_TPP__

namespace genetic {

	template<typename T>
	classic_ga<T>::classic_ga(problem<T>* problem):
	 algorithm<T>(problem) {

	}

	template<typename T>
	std::vector<solution<T>> classic_ga<T>::execute(
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

			this->iteration();

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

		auto result = std::vector<solution<T>>();
		result.push_back(this->best_solution());

		return result;
	}

	template<typename T>
	void classic_ga<T>::iteration() {
		this->assign_fitness();
		this->print_population();
		this->reproduce();
		this->mutate();
		this->compute_costs();
		this->clip_population();
	}

	template<typename T>
	void classic_ga<T>::compute_costs() {
		for (auto& solution: this->get_population().solutions()) {
			auto report = this->get_problem()->compute_cost(solution.item());
			auto costs  = report.first;
			auto trans  = report.second;

			float total_cost = 0.0;
			for (auto cost: costs)
				total_cost += cost;
				
			for (auto tra: trans)
				total_cost += tra;

			solution.costs(costs);
			solution.total_cost(total_cost);
		}
	}

	template<typename T>
	void classic_ga<T>::assign_fitness() {
		float total_cost = 0.0;
		for (auto& solution: this->get_population().solutions())
			total_cost += solution.total_cost();

		float total_fitness = 0;
		for (auto& solution: this->get_population().solutions()) {
			solution.fitness(
				1-solution.total_cost()/total_cost
			);

			total_fitness += solution.fitness();
		}

		for (auto& solution: this->get_population().solutions()) {
			solution.fitness(
				solution.fitness()/total_fitness
			);
		}
	}

} // namespace genetic

#endif