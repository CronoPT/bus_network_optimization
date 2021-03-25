#ifndef __ALGORITHM_HPP__
#define __ALGORITHM_HPP__ 

#include "population.hpp"
#include "problem.hpp"
#include "solution.hpp"
#include <cstdlib>

namespace genetic {
	
	template<typename T>
	class algorithm {

		private:
			population<T> _population;
			problem<T>*   _problem;
			float _crossover_prob;
			float _mutation_prob;

			bool do_crossover();
			bool do_mutate();
			int  select_solution();

		public:
			algorithm(problem<T>* problem);
			virtual solution<T> execute(
				int max_iterations, 
				float min_improv,
				int max_stalled
			) = 0;
			void initialize_population();
			virtual void iteration() const = 0;
			void compute_costs();
			void assign_fitness();
			void reproduce();
			std::pair<T, T> crossover(T i1, T i2);
			void mutate();
			void clip_population();

			solution<T> best_solution(); 
			void print_population();
		
	};

} // namespace genetic

#include "algorithm.tpp"

#endif