#ifndef __TRIP_HPP__
#define __TRIP_HPP__

#include <vector>
#include "stage.hpp"

namespace urban {

	class trip {
		std::vector<stage> _stages;
		float _passengers;
		int _origin;
		int _destin;
		int _transfers;
		std::pair<int, int> _origin_square;
		std::pair<int, int> _destin_square;
		float _time;

		public:
		trip(
			std::vector<stage>& stages,
			float passengers,
			int origin,
			int destin,
			std::pair<int, int> origin_square,
			std::pair<int, int> destin_square
		);
		trip();

		const std::vector<stage>& get_stages() const;
		float get_passengers() const;
		int get_origin() const;
		int get_destin() const;
		int get_transfers() const;
		std::pair<int, int> get_origin_square() const;
		std::pair<int, int> get_destin_square() const;
		float get_time() const;

	};

} // namepsace urban

#endif