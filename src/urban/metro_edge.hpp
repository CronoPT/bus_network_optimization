#ifndef __METRO_EDGE_HPP__
#define __METRO_EDGE_HPP__

#include <string>
#include <unordered_map>
namespace urban {

	class metro_edge {
		int _origin_id;
		int _destin_id;
		int _line_color;
		float _time_taken;

		public:
		metro_edge(
			int origin, 
			int destin, 
			int color, 
			float time_taken
		);
		metro_edge();

		int get_origin_id() const;
		int get_destin_id() const;
		int get_line_color() const;
		float get_time_taken() const;

	};

} // namespace urban

#endif