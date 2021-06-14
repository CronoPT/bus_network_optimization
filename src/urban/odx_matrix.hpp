#ifndef __ODX_MATRIX_HPP__
#define __ODX_MATRIX_HPP__

#include <unordered_map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include "configs.hpp"

namespace urban {

	class odx_pair_info {
		float _morning_rush_hour;
		float _midday;
		float _afternoon_rush_hour;
		float _happy_hour;
		float _night_time;
		float _total;

		public:
		odx_pair_info(
			float morning_rush_hour,
			float midday,
			float afternoon_rush_hour,
			float happy_hour,
			float night_time,
			float total
		);
		odx_pair_info();
		float get_morning_rush_hour() const;
		float get_midday() const;
		float get_afternoon_rush_hour() const;
		float get_happy_hour() const;
		float get_night_time() const;
		float get_total() const;
	};

	class odx_entry {
		std::unordered_map<int, std::unordered_map<int, odx_pair_info>> _destins;
		
		public:
		odx_entry();
		float get_morning_rush_hour(std::pair<int, int> destin);
		float get_midday(std::pair<int, int> destin);
		float get_afternoon_rush_hour(std::pair<int, int> destin);
		float get_happy_hour(std::pair<int, int> destin);
		float get_night_time(std::pair<int, int> destin);
		float get_total(std::pair<int, int> destin);
		void add_odx_pair_info(
			std::pair<int, int> destin, 
			odx_pair_info info
		);
		bool exists_destin(std::pair<int, int> destin);
	};

	class odx_matrix {

		static odx_matrix* _instance;

		std::unordered_map<int, std::unordered_map<int, odx_entry>> _origins;
		std::vector<std::pair<std::pair<int,int>, std::pair<int,int>>> _all_pairs;
		
		odx_matrix();

		public:
		static odx_matrix* instance();
		float get_morning_rush_hour(std::pair<int, int> origin, std::pair<int, int> destin);
		float get_midday(std::pair<int, int> origin, std::pair<int, int> destin);
		float get_afternoon_rush_hour(std::pair<int, int> origin, std::pair<int, int> destin);
		float get_happy_hour(std::pair<int, int> origin, std::pair<int, int> destin);
		float get_night_time(std::pair<int, int> origin, std::pair<int, int> destin);
		float get_total(std::pair<int, int> origin, std::pair<int, int> destin);
		float get_in_day_time(std::string day_time, std::pair<int, int> origin, std::pair<int, int> destin);
		void add_odx_pair_info(
			std::pair<int, int> origin, 
			std::pair<int, int> destin, 
			odx_pair_info info
		);
		std::vector<std::pair<std::pair<int,int>, std::pair<int,int>>>& get_all_pairs();
		bool travel_needed_between(std::pair<int, int> origin, std::pair<int, int> destin);
	};

} // namespace urban

#endif