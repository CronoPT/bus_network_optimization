#ifndef __VARIABLE_HPP__
#define __VARIABLE_HPP__

namespace genetic {

	template<typename T>
	class variable {

		public:
			virtual bool  in_accord(const T& sol) const = 0;
			virtual float transgression(const T& sol) const = 0;
			virtual float objective(const T& sol) const = 0;

	};

} // namespace genetic

#endif