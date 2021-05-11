#ifndef __COST_FUNCTION_HPP__
#define __COST_FUNCTION_HPP__ 

#include "solution.hpp"

namespace genetic {

	template<typename T>
	class cost_function {

		public:
			virtual float compute(T& sol) const = 0;

	};

} // namespace genetic

#endif