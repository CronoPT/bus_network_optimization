#ifndef __TNFSP_CPP__
#define __TNFSP_CPP__

#include "tnfsp.hpp"

namespace transit_problem {

	int generate_number_between(int low, int high) {
		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> dist(low, high);
		return dist(rng);
	}

	tnfsp::tnfsp(
		genetic::constraint_set<urban::frequency_set> constraints, 
	    genetic::cost_function_set<urban::frequency_set> cost_functions,
		urban::bus_network* network
	): problem(constraints, cost_functions), _network(network) {
		_network->evaluate();
	}

	std::vector<genetic::solution<urban::frequency_set>> 
	 tnfsp::initialize_population(int pop_size) {
		auto population = std::vector<genetic::solution<urban::frequency_set>>();

		const int low  = tnfsp_configs::min_frequency / tnfsp_configs::frequency_step;
		const int high = tnfsp_configs::max_frequency / tnfsp_configs::frequency_step;

		for (int i=0; i<pop_size; i++) {
			auto frequencies = std::vector<float>();
			
			for (int j=0; j<_network->get_number_routes(); j++) {
				frequencies.push_back(
					generate_number_between(low, high) * tnfsp_configs::frequency_step
				);
			}

			population.push_back(genetic::solution<urban::frequency_set>(
				urban::frequency_set(
					frequencies, 
					_network, 
					tnfsp_configs::operating_hours, 
					tnfsp_configs::day_time
				)
			));
		}

		std::cout << "Population Initialized" << std::endl;

		return population;
	}

	std::pair<urban::frequency_set, urban::frequency_set> tnfsp::crossover(
		urban::frequency_set& i1, urban::frequency_set& i2
	) {
		auto& f1 = i1.get_frequencies();
		auto& f2 = i2.get_frequencies();

		int min_size = (f1.size() < f2.size()) ?
		                f1.size() : f2.size();

		auto new_f1 = std::vector<float>();
		auto new_f2 = std::vector<float>();

		int crossover_point = generate_number_between(
			1, min_size-1	
		);

		int i;
		for (i=0 ; i<crossover_point; i++) {
			new_f1.push_back(f2.at(i));
			new_f2.push_back(f1.at(i));
		}
		for (    ; i<min_size; i++) {
			new_f1.push_back(f1.at(i));
			new_f2.push_back(f2.at(i));
		}
		if (min_size == f1.size()) { // first network had less routes
			for ( ; i<f2.size(); i++) {
				new_f2.push_back(f2.at(i));
			}
		} else { // second network had less routes
			for ( ; i<f1.size(); i++) {
				new_f1.push_back(f1.at(i));
			}		
		}

		auto new_set_1 = urban::frequency_set(
			new_f1, _network, tnfsp_configs::operating_hours, tnfsp_configs::day_time
		);
		auto new_set_2 = urban::frequency_set(
			new_f2, _network, tnfsp_configs::operating_hours, tnfsp_configs::day_time
		);

		return std::pair<urban::frequency_set, urban::frequency_set>(
			new_set_1, new_set_2
		);
	}

	void tnfsp::mutate(urban::frequency_set& item) const {
		int mutation_point = generate_number_between(
			0, item.get_frequencies().size()-1	
		);

		const int low  = tnfsp_configs::min_frequency / tnfsp_configs::frequency_step;
		const int high = tnfsp_configs::max_frequency / tnfsp_configs::frequency_step;

		auto new_f_value = generate_number_between(low, high) * tnfsp_configs::frequency_step;

		item.set_frequency_at(mutation_point, new_f_value);
	}

} // namespace transit_problem

#endif