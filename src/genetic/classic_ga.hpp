#ifndef __CLASSIC_GA_HPP__
#define __CLASSIC_GA_HPP__

#include "algorithm.hpp"
#include "logger.hpp"

namespace genetic {

	template<typename T>
	class classic_ga: public algorithm<T> {

		std::vector<float> _weights; // one size above the amount of objectives
		std::vector<float> _maxs;
		std::vector<float> _mins;
		bool _norm; // wether or not to min-max normalize the objectives
		bool _crossover_done;

		public:
			classic_ga(problem<T>* problem, std::vector<float> weights);
			classic_ga(
				problem<T>* problem, std::vector<float> weights,
				std::vector<float> maxs, std::vector<float> mins           
			);

			std::vector<solution<T>> execute() override;
			void iteration()      override;
			void compute_costs()  override;
			void assign_fitness() override;

			void reproduce() override;
			std::pair<T, T> crossover(T i1, T i2) override;

			void log_beginning();
			void log_iteration();

	};

} // namespace genetic

#include "classic_ga.tpp"

#endif