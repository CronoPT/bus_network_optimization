#ifndef __GENETIC_CONFIGS_CPP__
#define __GENETIC_CONFIGS_CPP__

namespace genetic {

	float genetic_configs::mutation_probability  = 0.1;
	float genetic_configs::crossover_probability = 1;
	int genetic_configs::population_size = 125;
	int genetic_configs::max_iterations  = 3;
	float genetic_configs::min_improv    = 0.0001;
	int   genetic_configs::max_stalled   = 30;
	std::string genetic_configs::summary_file = "../data/json/runs/tnfsp.json";
	bool genetic_configs::verbose = false;
	bool genetic_configs::threaded = true;
	int genetic_configs::number_threads = 8;
	
} // namespace genetic

#include "genetic_configs.tpp"

#endif