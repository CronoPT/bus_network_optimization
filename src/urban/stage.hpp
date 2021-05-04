#ifndef __STAGE_HPP__
#define __STAGE_HPP__

namespace urban {

	#define BUS_STAGE 0
	#define METRO_STAGE 1
	#define WALKING_STAGE 2

	class stage {
		int   _origin;
		int   _destin;
		int   _mode; // bus, metro or walking
		float _time;
		int _itinerary;

		public:
		stage(
			int origin, int destin, int mode, 
			float time, int itinerary
		);
		stage();

		int   get_origin() const;
		int   get_destin() const;
		int   get_mode()   const;
		float get_time()   const;
		int   get_itinerary() const;

	};

} // namespace urban

#endif