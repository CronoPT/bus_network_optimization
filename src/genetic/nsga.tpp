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
		std::cout << "[NSGA] Algorithm Starting" << std::endl;
		this->initialize_population();
		std::cout << "[NSGA] Initial Population Done" << std::endl;
		this->compute_costs();
		std::cout << "[NSGA] Costs for Initial Population Done" << std::endl;

		int iteration = 0;
		float cost_diff = min_improv+0.1;
		int insig_iters = 0;

		std::cout << "Max Stalled: " << max_stalled
		          << "\tInsignificant Iters: " << insig_iters << std::endl;

		while (iteration<max_iterations && insig_iters<max_stalled) {
			std::cout << "\n<<<<<<<<NEW POPULATION>>>>>>>>" << std::endl;

			float best_cost = this->get_best_solution().get_total_cost();
			std::cout << "[NSGA] Best solution total cost OK" << std::endl;
			this->iteration();
			std::cout << "[NSGA] Iteration OK" << std::endl;
			this->print_population();
			float new_best = this->get_best_solution().get_total_cost();

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
			this->get_population().get_solutions().begin(), 
			this->get_population().get_solutions().end(), 
			std::back_inserter(result), 
			[](solution<T> sol){ return sol.get_rank()==0; }
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
		
		for (auto& solution: this->get_population().get_solutions()) {
			solution.reset_dominates();
			solution.set_domination_count(0);
		}

		auto curr_front = std::vector<int>(); 
		for (int i=0; i<this->get_population().size(); i++) {
			auto& solution_i = this->get_population().get_solutions().at(i);
			for (int j=0; j<this->get_population().size(); j++) {
				auto& solution_j = this->get_population().get_solutions().at(j);
				if (solution_i.constrained_dominates(solution_j)) {
					solution_i.add_dominates(j);
				} else if (solution_j.constrained_dominates(solution_i)) {
					solution_i.inc_domination_count();
				}
			}
			if(solution_i.get_domination_count()==0) {
				solution_i.set_rank(0);
				curr_front.push_back(i);
			}
		}

		int curr_rank = 1;
		while(!curr_front.empty()) {
			auto next_front = std::vector<int>();
			for (auto i: curr_front) {
				auto& solution_i = this->get_population().get_solutions().at(i);
				for (auto j: solution_i.get_dominates()) {
					auto& solution_j = this->get_population().get_solutions().at(j);
					solution_j.dec_domination_count();
					if (solution_j.get_domination_count() == 0) {
						solution_j.set_rank(curr_rank);
						next_front.push_back(j);
					}
				}
			}

			++curr_rank;
			curr_front = next_front;
		}

	}

	template<typename T>
	void nsga<T>::crowding_distance_sorting() {
		this->get_population().reset_crowding_distance();

		for (int o=0; o<this->get_problem()->num_cost_functions(); o++) {
			auto& sols = this->get_population().get_solutions();
			std::sort(sols.begin(), sols.end(), [o](solution<T>& s1, solution<T>& s2)-> bool {
				return s1.get_costs().at(o) < s2.get_costs().at(o);
			});

			int pop_size = this->get_population().size();

			float min = this->get_population().get_solutions().at(0).get_costs().at(o);
			float max = this->get_population().get_solutions().at(pop_size-1).get_costs().at(o);

			this->get_population().get_solutions().at(0).set_crowding_distance(
				std::numeric_limits<float>::infinity()
			);

			this->get_population().get_solutions().at(pop_size-1).set_crowding_distance(
				std::numeric_limits<float>::infinity()
			);

			for (int i=1; i<pop_size-1; i++) {
				auto& solution = this->get_population().get_solutions().at(i);
				auto& solution_before = this->get_population().get_solutions().at(i-1);
				auto& solution_after  = this->get_population().get_solutions().at(i+1);
				solution.add_crowding_distance(
					(solution_after.get_costs().at(o) - solution_before.get_costs().at(o)) /
					(max-min)
				);
			}

		}

		auto& sols = this->get_population().get_solutions();
		std::sort(sols.begin(), sols.end(), [](solution<T>& s1, solution<T>& s2)-> bool {
			if (s1.get_rank() < s2.get_rank()) {
				return true;
			} else if ((s1.get_rank()==s2.get_rank()) && (s1.get_crowding_distance()>s2.get_crowding_distance())) {
				return true;
			} else { return false; }
		});
		
	}

	template<typename T>
	void nsga<T>::compute_costs() {
		for (auto& solution: this->get_population().get_solutions()) {
			auto report = this->get_problem()->compute_cost(solution.get_item());
			auto costs  = report.first;
			auto trans  = report.second;

			float total_cost = 0.0;
			for (auto cost: costs)
				total_cost += cost;
			
			float total_tran = 0.0;
			for (auto tra: trans) {
				total_tran += tra;
			}

			solution.set_costs(costs);
			solution.set_transgressions(trans);
			solution.set_total_cost(total_cost);
			solution.set_total_transgression(total_tran);
		}
	}

	template<typename T>
	void nsga<T>::assign_fitness() {
		float total_rank = 0.0;
		for (auto& solution: this->get_population().get_solutions()) {
			if(solution.has_rank()) {
				total_rank += solution.get_rank()+solution.get_crowding_distance();
			} else {
				total_rank += 1;
			}
		}
			
		float total_fitness = 0;
		for (auto& solution: this->get_population().get_solutions()) {
			solution.set_fitness(
				1-solution.get_rank()/total_rank
			);

			total_fitness += solution.get_fitness();
		}

		for (auto& solution: this->get_population().get_solutions()) {
			solution.set_fitness(
				solution.get_fitness()/total_fitness
			);
		}
	}

} // namespace genetic

#endif