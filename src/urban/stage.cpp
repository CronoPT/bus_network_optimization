#ifndef __STAGE_CPP__
#define __STAGE_CPP__

#include "stage.hpp"

namespace urban {

	stage::stage(
		int origin, int destin, int mode, 
		float time, int itinerary
	):
	 _origin(origin), _destin(destin),
	 _mode(mode), _time(time), 
	 _itinerary(itinerary) {
		/* Do Nothing */
	}

	stage::stage():
	 _origin(-1), _destin(-1),
	 _mode(-1), _time(-1),
	 _itinerary(-1) {
		/* Do Nothing */
	}

	int stage::get_origin() const {
		return _origin;
	}

	int stage::get_destin() const {
		return _destin;
	}

	int stage::get_mode() const {
		return _mode;
	}

	float stage::get_time() const {
		return _time;
	}

	int stage::get_itinerary() const {
		return _itinerary;
	}

} //  namespace urban

#endif