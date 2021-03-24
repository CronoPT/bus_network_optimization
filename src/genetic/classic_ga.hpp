#ifndef __CLASSIC_GA_HPP__
#define __CLASSIC_GA_HPP__

#include "algorithm.hpp"

namespace genetic {

	template<typename T>
	class classic_ga: public algorithm<T> {

		public:
			classic_ga(problem<T>* problem);

			solution<T> execute(
				int max_iterations, 
				float min_improv
			) override;
			void iteration() const override;

	};

} // namespace genetic

#include "classic_ga.tpp"

#endif