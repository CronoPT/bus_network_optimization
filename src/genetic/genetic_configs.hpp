#ifndef __GENETIC_CONFIGS_HPP__
#define __GENETIC_CONFIGS_HPP__

namespace genetic {

	class genetic_configs {
		public:
		static float mutation_probability;
		static float crossover_probability;
		static int population_size;
		static int max_iterations;

		/**
		 * How much the best solution should improve
		 * between iterations for the algorithm to
		 * not consider calling convergence
		 * Used only on the CLASSIC GA
		*/
		static float min_improv;

		/**
		 * How many iterations without significant 
		 * improvements the algorithm should allow
		 * before admiting convergence.
		 * Used only on the CLASSIC GA
		*/
		static int 	 max_stalled;
	};

} // namespace genetic

#include "genetic_configs.tpp"

#endif