#ifndef __POPULATION_HPP__
#define __POPULATION_HPP__ 

#include <set>
#include <vector>
#include <algorithm>
#include <cmath>
#include "solution.hpp"

namespace genetic {

	template<typename T>
	class population {

		private:
			std::vector<solution<T>> _solutions;

		public:
			population();
			population(std::vector<solution<T>>  sols);

			void add_solution(solution<T>& sol);
			void add_solution(T& sol);
			void clip();
			solution<T>& nth_best(int n);
			std::vector<solution<T>>& solutions();
			
	};

} // namespace genetic

#include "population.tpp"

#endif