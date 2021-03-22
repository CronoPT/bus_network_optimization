#include <queue>
#include <vector>
#include "solution.hpp"

namespace genetic {

	template<typename T>
	class population {

		private:
			std::priority_queue<solution<T>> _solutions;

		public:
			population(std::vector<solution<T>>& sols);

			void compute_costs();
			void assign_fitness();
			
	};

} // namespace genetic