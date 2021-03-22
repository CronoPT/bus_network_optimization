#ifndef __SOLUTION_HPP__
#define __SOLUTION_HPP__ 

#include <vector>
#include <iostream>

namespace genetic {
	
	template<typename T>
	class solution {

		private:
			float _total_cost;
			float _fitness;
			std::vector<float> _costs;
			T _item;

		public:
			solution(const T item);

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

			bool operator<(const solution<T>& other) const; 
			virtual void dump_to(std::ostream& os) const = 0;
		
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const solution<T>& a); 

} // namespace genetic

#endif