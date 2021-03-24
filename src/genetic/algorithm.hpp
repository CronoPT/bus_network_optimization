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

			bool crossover() const;
			bool mutate() const;
			int  select_solution() const;

		public:
			algorithm(problem<T>* problem);
			virtual solution<T> execute(
				int max_iterations, 
				float min_improv
			) const = 0;
			void initialize_population();
			virtual void iteration() const = 0;
			void compute_costs();
			void assign_fitness();
			void reproduce();
			std::pair<T, T> crossover(T& i1, T& i2);
			void mutate(); 
		
	};

} // namespace genetic

#include "algorithm.tpp"

#endif