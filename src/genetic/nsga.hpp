#ifndef __NSGA_HPP__
#define __NSGA_HPP__

#include "algorithm.hpp"
#include "cost_function.hpp"
#include "logger.hpp"
#include <limits>
#include <algorithm>
#include <thread>
#include <sstream>

namespace genetic {

	template<typename T>
	class nsga: public algorithm<T> {

		public:
			nsga(problem<T>* problem);

			std::vector<solution<T>> execute() override;
			void iteration()      override;
			void compute_costs()  override;
			void assign_fitness() override;
			void nondominated_sorting();
			void crowding_distance_sorting();

			void log_beginning();
			void log_iteration();

			void compute_costs_single_thread();
			void compute_costs_multi_threads();
	};

} // namespace genetic

#include "nsga.tpp"

#endif