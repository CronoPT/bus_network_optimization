#ifndef __ODX_MATRIX_HPP__
#define __ODX_MATRIX_HPP__

#include <unordered_map>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include "configs.hpp"

namespace urban {

	class odx_pair_info {
		int _morning_rush_hour;
		int _midday;
		int _afternoon_rush_hour;
		int _happy_hour;
		int _night_time;
		int _total;

		public:
		odx_pair_info(
			int morning_rush_hour,
			int midday,
			int afternoon_rush_hour,
			int happy_hour,
			int night_time,
			int total
		);
		odx_pair_info();
		int get_morning_rush_hour() const;
		int get_midday() const;
		int get_afternoon_rush_hour() const;
		int get_happy_hour() const;
		int get_night_time() const;
		int get_total() const;
	};

	class odx_entry {
		std::unordered_map<int, std::unordered_map<int, odx_pair_info>> _destins;
		
		public:
		odx_entry();
		int get_morning_rush_hour(std::pair<int, int> destin);
		int get_midday(std::pair<int, int> destin);
		int get_afternoon_rush_hour(std::pair<int, int> destin);
		int get_happy_hour(std::pair<int, int> destin);
		int get_night_time(std::pair<int, int> destin);
		int get_total(std::pair<int, int> destin);
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
		int get_morning_rush_hour(std::pair<int, int> origin, std::pair<int, int> destin);
		int get_midday(std::pair<int, int> origin, std::pair<int, int> destin);
		int get_afternoon_rush_hour(std::pair<int, int> origin, std::pair<int, int> destin);
		int get_happy_hour(std::pair<int, int> origin, std::pair<int, int> destin);
		int get_night_time(std::pair<int, int> origin, std::pair<int, int> destin);
		int get_total(std::pair<int, int> origin, std::pair<int, int> destin);
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