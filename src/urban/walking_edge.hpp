#ifndef __WALKING_EDGE_HPP_
#define __WALKING_EDGE_HPP_

#include <string>

namespace urban {

	class walking_edge {
		int _origin_id;
		int _destin_id;
		float _time_taken;
		std::string _origin_type;
		std::string _destin_type;

		public:
		walking_edge(
			int origin_id,
			int destin_id,
			float time_taken,
			std::string origin_type,
			std::string destin_type
		);
		walking_edge();

		int get_origin_id()  const;
		int get_destin_id()  const;
		int get_time_taken() const;
		const std::string& get_origin_type() const;
		const std::string& get_destin_type() const;
	};

} // namespace urban 

#endif