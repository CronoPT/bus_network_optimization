#ifndef __SOLUTION_HPP__
#define __SOLUTION_HPP__ 

#include <vector>
#include <iostream>
#include <algorithm>

namespace genetic {
	
	template<typename T>
	class solution {

		private:
			float _total_cost;
			float _total_transgression;
			float _fitness;
			std::vector<float> _costs;
			std::vector<float> _transgressions;
			T _item;
			int _rank;
			int _domination_count;
			std::vector<int> _dominates;
			float _crowding_distance;

		public:
			solution(const T& item);
			solution();

			//getters
			float get_total_cost() const;
			float get_total_transgression() const;
			float get_fitness() const;
			std::vector<float>& get_costs();
			std::vector<float>& get_transgressions();
			T& get_item();
			int get_rank() const;
			int get_domination_count() const;
			std::vector<int>& get_dominates();
			float get_crowding_distance();

			//setters
			void set_total_cost(float new_total);
			void set_total_transgression(float new_total);
			void set_fitness(float new_fitness);
			void set_costs(const std::vector<float>& new_costs);
			void set_transgressions(const std::vector<float>& new_trans);
			void set_item(const T& new_item);
			void set_rank(int new_rank);
			void set_domination_count(int new_count);
			void set_dominates(std::vector<int>& new_dominates);
			void set_crowding_distance(float new_distance);

			void inc_domination_count();
			void dec_domination_count();
			void add_dominates(int dominated);
			void rem_dominates(int dominated);
			void reset_rank();
			bool has_rank();
			bool dominates(int other);

			bool dominates(solution<T>& other);
			bool constrained_dominates(solution<T>& other);
			void reset_crowding_distance();
			void add_crowding_distance(float increment);
			bool is_feasible();
			void reset_dominates();

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