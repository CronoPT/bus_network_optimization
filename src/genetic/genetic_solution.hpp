#ifndef __GENETIC_SOLUTION_HPP__
#define __GENETIC_SOLUTION_HPP__ 

#include <vector>
#include <iostream>

namespace genetic {
	
	template<typename T>
	class genetic_solution {

		private:
			float _total_cost;
			float _fitness;
			std::vector<float> _costs;
			T _item;

		public:
			genetic_solution(const T item);

			//getters
			float total_cost() const;
			float fitness() const;
			const std::vector<float>& costs() const;
			const T& item() const;

			//setters
			void total_cost(float new_total);
			void fitness(float new_fitness);
			void costs(const std::vector<float>& new_costs);
			void item(const T& new_item);

			virtual void dump_to(std::ostream& os) const = 0;
		
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const genetic_solution<T>& a); 

} // namespace genetic

#endif