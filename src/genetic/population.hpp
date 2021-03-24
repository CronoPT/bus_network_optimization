#ifndef __POPULATION_HPP__
#define __POPULATION_HPP__ 

#include <set>
#include <vector>
#include <algorithm>
#include <cmath>
#include "solution.hpp"

namespace genetic {

	template<typename T>
	using solution_set = std::vector<solution<T>>; 

	template<typename T>
	class population {

		private:
			solution_set<T> _solutions;

		public:
			population();
			population(const solution_set<T>& sols);

			void add_solution(const solution<T>& sol);
			void add_solution(const T& sol);
			void clip();
			solution<T>& nth_best(int n);
			solution_set<T>& solutions();
			
	};

} // namespace genetic

#include "population.tpp"

#endif