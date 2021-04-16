#ifndef __BUS_EDGE_HPP__
#define __BUS_EDGE_HPP__

namespace urban {

	class bus_edge {
		int _origin_id;
		int _destin_id;
		int _route_id;
		float _time_taken;

		public:
		bus_edge(int origin, int destin, int route, float time);
		bus_edge(); 

		int get_origin_id() const;
		int get_destin_id() const;
		int get_route_id()  const;
		float get_time_taken() const;
	};

} // namespace urban

#endif