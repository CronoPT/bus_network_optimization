#ifndef __ROUTE_HPP__
#define __ROUTE_HPP__

#include <vector>

namespace urban {

	class route {
		int _route_id;
		std::vector<int> _stop_sequence;

		public:
		route(int route_id, std::vector<int>& stop_sequence);
		route();

		int get_route_id() const;
		const std::vector<int>& get_stop_sequence() const;
		int size() const; 
	};

} // namespace urban

#endif