#ifndef __GENETIC_CONFIGS_CPP__
#define __GENETIC_CONFIGS_CPP__

namespace genetic {

	float genetic_configs::mutation_probability  = 1;
	float genetic_configs::crossover_probability = 0;
	int genetic_configs::population_size = 5;
	int genetic_configs::max_iterations  = 5;
	float genetic_configs::min_improv    = 0.0001;
	int   genetic_configs::max_stalled   = 30;

} // namespace genetic

#include "genetic_configs.tpp"

#endif