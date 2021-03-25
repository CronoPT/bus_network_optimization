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
			int _rank;
			int _domination_count;
			std::vector<int> _dominates;

		public:
			solution(const T& item);
			solution();

			//getters
			float total_cost() const;
			float fitness() const;
			std::vector<float>& costs();
			T& item();
			int rank() const;
			int domination_count() const;
			std::vector<int>& dominates();

			//setters
			void total_cost(float new_total);
			void fitness(float new_fitness);
			void costs(const std::vector<float>& new_costs);
			void item(const T& new_item);
			void rank(int new_rank);
			void domination_count(int new_count);
			void dominates(std::vector<int>& new_dominates); 

			bool dominates(solution<T>& other);

		template<typename C>
		friend bool operator<(const solution<C>& s1, const solution<C>& s2);

		template<typename C>
		friend bool operator>(const solution<C>& s1, const solution<C>& s2);

		template<typename C>
		friend bool operator==(const solution<C>& s1, const solution<C>& s2);

		template<typename C>
		friend std::ostream& operator<<(std::ostream& os, const solution<C>& a); 
	};
	


} // namespace genetic

#include "solution.tpp"

#endif