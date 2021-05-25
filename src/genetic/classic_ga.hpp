#ifndef __CLASSIC_GA_HPP__
#define __CLASSIC_GA_HPP__

#include "algorithm.hpp"

namespace genetic {

	template<typename T>
	class classic_ga: public algorithm<T> {

		public:
			classic_ga(problem<T>* problem);

			std::vector<solution<T>> execute() override;
			void iteration()      override;
			void compute_costs()  override;
			void assign_fitness() override;

	};

} // namespace genetic

#include "classic_ga.tpp"

#endif