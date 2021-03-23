#ifndef __CONSTRAINT_HPP__
#define __CONSTRAINT_HPP__ 

#include "solution.hpp"

namespace genetic {

	template<typename T>
	class constraint {

		public:
			virtual bool  satisfied(const T& sol)     const = 0;
			virtual float transgression(const T& sol) const = 0;

	};

} // namespace genetic

#endif