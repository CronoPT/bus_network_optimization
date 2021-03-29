#ifndef __NSGA_TPP__
#define __NSGA_TPP__

namespace genetic {

	template<typename T>
	nsga<T>::nsga(problem<T>* problem):
	 algorithm<T>(problem) {

	}

	template<typename T>
	std::vector<solution<T>> nsga<T>::execute(
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
			this->print_population();
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

		std::vector<solution<T>> result;
		std::copy_if(
			this->get_population().solutions().begin(), 
			this->get_population().solutions().end(), 
			std::back_inserter(result), 
			[](solution<T> sol){ return sol.rank()==0; }
		);

		return result;
	}

	template<typename T>
	void nsga<T>::iteration() {
		this->assign_fitness();
		this->reproduce();
		this->mutate();
		this->compute_costs();
		this->nondominated_sorting();
		this->crowding_distance_sorting();
		this->clip_population();
	}

	template<typename T>
	void nsga<T>::nondominated_sorting() {
		for (int i=0; i<this->get_population().size(); i++) {
			for (int j=0; j<this->get_population().size(); j++) {
				auto& solution_i = this->get_population().solutions().at(i);
				auto& solution_j = this->get_population().solutions().at(j);
				if (solution_i.dominates(solution_j)) {
					solution_i.add_dominates(j);
					solution_j.inc_domination_count();
				}
			}
		}

		this->get_population().reset_ranks();
		bool assigned_rank = true;
		int  curr_rank = 0;
		while (assigned_rank) {
			assigned_rank = false;
			
			for (int i=0; i<this->get_population().size(); i++) {
				auto& solution_i = this->get_population().solutions().at(i);
				if (solution_i.domination_count() == 0 && !solution_i.has_rank()) {
					solution_i.rank(curr_rank);
					assigned_rank = true;
					for (int j=0; j<this->get_population().size(); j++) {
						auto& solution_j = this->get_population().solutions().at(j);
						if (solution_i.dominates(j)) {
							solution_i.rem_dominates(j);
							solution_j.dec_domination_count();
						}
					}
				}
			}

			++curr_rank;
		}

	}

	template<typename T>
	void nsga<T>::crowding_distance_sorting() {
		this->get_population().reset_crowding_distance();

		for (int o=0; o<this->get_problem()->num_variables(); o++) {
			auto& sols = this->get_population().solutions();
			std::sort(sols.begin(), sols.end(), [o](solution<T>& s1, solution<T>& s2)-> bool {
				return s1.costs().at(o) < s2.costs().at(o);
			});

			int pop_size = this->get_population().size();

			float min = this->get_population().solutions().at(0).costs().at(o);
			float max = this->get_population().solutions().at(pop_size-1).costs().at(o);

			this->get_population().solutions().at(0).crowding_distance(
				std::numeric_limits<float>::infinity()
			);

			this->get_population().solutions().at(pop_size-1).crowding_distance(
				std::numeric_limits<float>::infinity()
			);

			for (int i=1; i<pop_size-1; i++) {
				auto& solution = this->get_population().solutions().at(i);
				auto& solution_before = this->get_population().solutions().at(i-1);
				auto& solution_after  = this->get_population().solutions().at(i+1);
				solution.add_crowding_distance(
					(solution_after.costs().at(o) - solution_before.costs().at(o)) /
					(max-min)
				);
			}

		}

		auto& sols = this->get_population().solutions();
		std::sort(sols.begin(), sols.end(), [](solution<T>& s1, solution<T>& s2)-> bool {
			if (s1.rank() < s2.rank()) {
				return true;
			} else if ((s1.rank()==s2.rank()) && (s1.crowding_distance()>s2.crowding_distance())) {
				return true;
			} else { return false; }
		});
		
	}

	template<typename T>
	void nsga<T>::compute_costs() {
		for (auto& solution: this->get_population().solutions()) {
			auto costs = this->get_problem()->compute_cost(solution.item());
			
			float total_cost = 0.0;
			for (auto cost: costs)
				total_cost += cost;

			solution.costs(costs);
			solution.total_cost(total_cost);
		}
	}

	template<typename T>
	void nsga<T>::assign_fitness() {
		float total_rank = 0.0;
		for (auto& solution: this->get_population().solutions())
			total_rank += solution.rank()+1;

		float total_fitness = 0;
		for (auto& solution: this->get_population().solutions()) {
			solution.fitness(
				1-solution.rank()/total_rank
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