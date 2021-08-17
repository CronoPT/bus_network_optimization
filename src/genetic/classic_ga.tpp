#ifndef __CLASSIC_GA_TPP__
#define __CLASSIC_GA_TPP__

namespace genetic {

	void print_progress_bar(int iteration, int total, std::string suffix);

	template<typename T>
	classic_ga<T>::classic_ga(problem<T>* problem, std::vector<float> weights):
	 algorithm<T>(problem),
	 _weights(weights),
	 _maxs(),
	 _mins(),
	 _norm(false) {

	}

	template<typename T>
	classic_ga<T>::classic_ga(
		problem<T>* problem, std::vector<float> weights,
		std::vector<float> maxs, std::vector<float> mins           
	):
	 algorithm<T>(problem),
	 _weights(weights),
	 _maxs(maxs),
	 _mins(mins),
	 _norm(true) {

	}

	template<typename T>
	std::vector<solution<T>> classic_ga<T>::execute() {
		std::cout << "iterations: " << genetic_configs::max_iterations << std::endl;
		std::cout << "population_size: " << genetic_configs::population_size << std::endl;
		std::cout << "crossover_prob: " << genetic_configs::crossover_probability << std::endl;
		std::cout << "mutation_prob: " << genetic_configs::mutation_probability << std::endl;

		this->initialize_population();
		this->compute_costs();

		this->log_beginning();

		int iteration = 0;
		float cost_diff = genetic_configs::min_improv+0.1;
		int insig_iters = 0;

		if (genetic_configs::verbose)
			std::cout << "[GA] Max Stalled: " << genetic_configs::max_stalled
					<< "\tInsignificant Iters: " << insig_iters << std::endl;

		while (
			iteration<genetic_configs::max_iterations && 
			insig_iters<genetic_configs::max_stalled
		) {
			if (genetic_configs::verbose) {
				std::cout << "\n<<<<<<<<NEW POPULATION>>>>>>>>" << std::endl;
			} else {
				int iterations_left = (genetic_configs::max_iterations-1)-iteration;
				float minutes_left = (((float)genetic_configs::population_size/(float)genetic_configs::number_threads) 
				                     * 15 * iterations_left) / 60;
				std::string suffix = "Expected time remaining " + std::to_string((int)std::ceil(minutes_left));
				print_progress_bar(iteration+1, genetic_configs::max_iterations, suffix);
			}
			
			float best_cost = this->get_best_solution().get_total_cost();

			this->iteration();

			float new_best = this->get_best_solution().get_total_cost();
			cost_diff = (best_cost-new_best)/best_cost;
			if (cost_diff < genetic_configs::min_improv) { ++insig_iters; } 
			else { insig_iters = 0; }

			iteration += 1;
		}

		if (genetic_configs::verbose) {
			std::cout << "\nClassic GA is finished!" << std::endl;
			std::cout << "Iterations taken: " << iteration << std::endl;
			std::cout << "Cost diff from previous iteration: " << cost_diff
					<< "\n" << std::endl;
		}

		auto result = std::vector<solution<T>>();
		result.push_back(this->get_best_solution());

		logger::instance()->persist();

		return result;
	}

	template<typename T>
	void classic_ga<T>::iteration() {
		this->assign_fitness();
		
		if (genetic_configs::verbose)
			this->print_population();
		
		this->reproduce();
		this->mutate();
		this->compute_costs();
		this->clip_population();

		this->log_iteration();
	}

	template<typename T>
	void classic_ga<T>::compute_costs() {
		for (auto& solution: this->get_population().get_solutions()) {
			auto report = this->get_problem()->compute_cost(solution.get_item());
			auto costs  = report.first;
			auto trans  = report.second;

			float total_cost = _weights.at(0);
			for (int i=0; i<costs.size(); i++) {
				float cost = costs.at(i);
				
				if (_norm) {
					cost = (cost - _mins.at(i)) / (_maxs.at(i) - _mins.at(i));
				}

				total_cost += _weights.at(i+1)*cost;
			}
				
			for (auto tra: trans)
				total_cost += tra;

			solution.set_costs(costs);
			solution.set_total_cost(total_cost);
		}
	}

	template<typename T>
	void classic_ga<T>::assign_fitness() {
		float total_cost = 0.0;
		for (auto& solution: this->get_population().get_solutions())
			total_cost += solution.get_total_cost();

		float total_fitness = 0;
		for (auto& solution: this->get_population().get_solutions()) {
			solution.set_fitness(
				1-solution.get_total_cost()/total_cost
			);

			total_fitness += solution.get_fitness();
		}

		for (auto& solution: this->get_population().get_solutions()) {
			solution.set_fitness(
				solution.get_fitness()/total_fitness
			);
		}
	}

	template<typename T>
	void classic_ga<T>::log_beginning() {
		logger::instance()->mark_pop_size(this->get_population().size());
		logger::instance()->mark_crossover_prob(genetic_configs::crossover_probability);
		logger::instance()->mark_mutation_prob(genetic_configs::mutation_probability);
		log_iteration();
	}

	template<typename T>
	void classic_ga<T>::log_iteration() {
		for (auto& solution: this->get_population().get_solutions()) {
			auto report = this->get_problem()->compute_cost(solution.get_item());
			auto costs  = report.first;
			logger::instance()->add_individual(costs);
		}
		logger::instance()->end_iteration();
	}

	template<typename T>
	void classic_ga<T>::reproduce() {
		auto crossovers = std::vector<std::pair<int, int>>();
		for (int i=0; i<this->get_population().get_solutions().size()/2; i++) {
			int sol1 = this->select_solution();
			int sol2 = this->select_solution();
			crossovers.push_back(std::pair<int, int>(sol1, sol2));
		}

		for (auto pair: crossovers) {
			auto new_pair = crossover(
				this->get_population().get_solutions().at(pair.first).get_item(),
				this->get_population().get_solutions().at(pair.second).get_item()
			);
			if (_crossover_done) {
				this->get_population().add_solution(new_pair.first);
				this->get_population().add_solution(new_pair.second);
			}

		}

	}

	template<typename T>
	std::pair<T, T> classic_ga<T>::crossover(T i1, T i2) {
		if (this->do_crossover()) {
			_crossover_done = true;
			return this->get_problem()->crossover(i1, i2);
		} else {
			_crossover_done = false;
			return std::pair<T, T>(i1, i2);
		}
	}


} // namespace genetic

#endif