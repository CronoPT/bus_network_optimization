#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <vector>
#include <fstream>
#include "genetic_configs.hpp"

namespace genetic {

	class logger {
		
		std::vector<std::vector<std::vector<float>>> _summary;
		std::vector<std::vector<float>> _curr_iteration;
		int _pop_size;
		float _mutation_prob;
		float _crossover_prob;

		static logger* _instance;
		logger();

		public:
		static logger* instance();
		void end_iteration();
		void add_individual(std::vector<float> individual);
		void mark_pop_size(int size);
		void mark_mutation_prob(float prob);
		void mark_crossover_prob(float prob);

		void persist();
	};

} //namespace genetic

#include "logger.tpp"

#endif