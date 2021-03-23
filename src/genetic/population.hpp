#ifndef __POPULATION_HPP__
#define __POPULATION_HPP__ 

#include <set>
#include <vector>
#include "solution.hpp"

namespace genetic {

	template<typename T>
	class population {

		private:
			std::set<
				solution<T>, 
				std::greater<solution<T>>
			> _solutions;

		public:
			population();
			population(std::vector<solution<T>>& sols);

			void compute_costs();
			void assign_fitness();
			void add_solution(const solution<T>& sol);
			solution<T> nth_best(int n) const;
			
	};

} // namespace genetic

#include "population.tpp"

#endif