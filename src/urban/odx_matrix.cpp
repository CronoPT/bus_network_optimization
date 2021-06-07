#ifndef __ODX_MATRIX_CPP__
#define __ODX_MATRIX_CPP__

#include "odx_matrix.hpp"
#include <iostream>

namespace urban {

	odx_pair_info::odx_pair_info(
		int morning_rush_hour,
		int midday,
		int afternoon_rush_hour,
		int happy_hour,
		int night_time,
		int total
	):
	 _morning_rush_hour(morning_rush_hour),
	 _midday(midday),
	 _afternoon_rush_hour(afternoon_rush_hour),
	 _happy_hour(happy_hour),
	 _night_time(night_time),
	 _total(total) {
		/* Do Nothing*/
	}

	odx_pair_info::odx_pair_info():
	 _morning_rush_hour(-1),
	 _midday(-1),
	 _afternoon_rush_hour(-1),
	 _happy_hour(-1),
	 _night_time(-1),
	 _total(-1) {
		/* Do Nothing*/
	}

	int odx_pair_info::get_morning_rush_hour() const {
		return _morning_rush_hour;
	}

	int odx_pair_info::get_midday() const {
		return _midday;
	}

	int odx_pair_info::get_afternoon_rush_hour() const {
		return _afternoon_rush_hour;
	}

	int odx_pair_info::get_happy_hour() const {
		return _happy_hour;
	}

	int odx_pair_info::get_night_time() const {
		return _night_time;
	}

	int odx_pair_info::get_total() const {
		return _total;
	}

	odx_entry::odx_entry(): _destins() { /* Do Nothing */ }

	int odx_entry::get_morning_rush_hour(std::pair<int, int> destin) {
		int i = destin.first;
		int j = destin.second;
		return _destins[i][j].get_morning_rush_hour();
	}

	int odx_entry::get_midday(std::pair<int, int> destin) {
		int i = destin.first;
		int j = destin.second;
		return _destins[i][j].get_midday();
	}

	int odx_entry::get_afternoon_rush_hour(std::pair<int, int> destin) {
		int i = destin.first;
		int j = destin.second;
		return _destins[i][j].get_afternoon_rush_hour();
	}

	int odx_entry::get_happy_hour(std::pair<int, int> destin) {
		int i = destin.first;
		int j = destin.second;
		return _destins[i][j].get_happy_hour();
	}

	int odx_entry::get_night_time(std::pair<int, int> destin) {
		int i = destin.first;
		int j = destin.second;
		return _destins[i][j].get_night_time();
	}

	int odx_entry::get_total(std::pair<int, int> destin) {
		int i = destin.first;
		int j = destin.second;
		return _destins[i][j].get_total();
	}

	void odx_entry::add_odx_pair_info(
		std::pair<int, int> destin, 
		odx_pair_info info
	) {
		int i = destin.first;
		int j = destin.second;
		_destins[i][j] = info;
	}

	bool odx_entry::exists_destin(std::pair<int, int> destin) {
		auto destin_i = destin.first;
		auto destin_j = destin.second;
		bool exists_column = _destins.find(destin_i) != _destins.end();
		if (!exists_column) { return false; }
		bool exists_square = _destins.find(destin_i)->second.find(destin_j) != 
		                     _destins.find(destin_i)->second.end();
		return exists_square;
	}


	odx_matrix::odx_matrix(): _origins(), _all_pairs() { 
		std::ifstream input_file(configs::odx_matrix_data);
		nlohmann::json json_obj = nlohmann::json::parse(input_file);

		for (auto& info: json_obj) {
			auto origin = std::pair<int, int>({
				info["origin"].at(0),
				info["origin"].at(1)
			});
			auto destin = std::pair<int, int>({
				info["destin"].at(0),
				info["destin"].at(1)
			});
			int morning_rush_hour = info["morning_rush_hour"];
			int midday = info["midday"];
			int afternoon_rush_hour = info["afternoon_rush_hour"];
			int happy_hour = info["happy_hour"];
			int night_time = info["night_time"];
			int total = info["total"];
			auto odx_info = odx_pair_info(
				morning_rush_hour,
				midday,
				afternoon_rush_hour,
				happy_hour,
				night_time,
				total
			);
			add_odx_pair_info(origin, destin, odx_info);
			_all_pairs.push_back(
				std::pair<std::pair<int, int>, std::pair<int, int>>(
					origin, destin
				)
			);
		}

	}

	odx_matrix* odx_matrix::instance() {
		if (!_instance) {
			_instance = new odx_matrix();
		}
		return _instance;
	}

	int odx_matrix::get_morning_rush_hour(std::pair<int, int> origin, std::pair<int, int> destin) {
		int i = origin.first;
		int j = origin.second;
		return _origins[i][j].get_morning_rush_hour(destin);
	}

	int odx_matrix::get_midday(std::pair<int, int> origin, std::pair<int, int> destin) {
		int i = origin.first;
		int j = origin.second;
		return _origins[i][j].get_midday(destin);
	}

	int odx_matrix::get_afternoon_rush_hour(std::pair<int, int> origin, std::pair<int, int> destin) {
		int i = origin.first;
		int j = origin.second;
		return _origins[i][j].get_afternoon_rush_hour(destin);
	}

	int odx_matrix::get_happy_hour(std::pair<int, int> origin, std::pair<int, int> destin) {
		int i = origin.first;
		int j = origin.second;
		return _origins[i][j].get_happy_hour(destin);
	}

	int odx_matrix::get_night_time(std::pair<int, int> origin, std::pair<int, int> destin) {
		int i = origin.first;
		int j = origin.second;
		return _origins[i][j].get_night_time(destin);
	}

	int odx_matrix::get_total(std::pair<int, int> origin, std::pair<int, int> destin) {
		int i = origin.first;
		int j = origin.second;
		return _origins[i][j].get_total(destin);
	}

	int odx_matrix::get_in_day_time(
		std::string day_time, 
		std::pair<int, int> origin, 
		std::pair<int, int> destin
	) {
		if (day_time == "morning_rush_hour") {
			return get_morning_rush_hour(origin, destin);
		} else if (day_time == "midday") {
			return get_midday(origin, destin);
		} else if (day_time == "afternoon_rush_hour") {
			return get_afternoon_rush_hour(origin, destin);
		} else if (day_time == "happy_hour") {
			return get_happy_hour(origin, destin);
		} else if (day_time == "night_time") {
			return get_night_time(origin, destin);
		} else {
			return get_total(origin, destin);
		}
 	}

	void odx_matrix::add_odx_pair_info(
		std::pair<int, int> origin, 
		std::pair<int, int> destin, 
		odx_pair_info info
	) {
		int i = origin.first;
		int j = origin.second;
		_origins[i][j].add_odx_pair_info(destin, info);
	}

	std::vector<
		std::pair<std::pair<int,int>, std::pair<int,int>>
	>& odx_matrix::get_all_pairs() {
		return _all_pairs;
	}

	bool odx_matrix::travel_needed_between(
		std::pair<int, int> origin, 
		std::pair<int, int> destin
	) {
		auto origin_i = origin.first;
		auto origin_j = origin.second;
		bool exists_column = _origins.find(origin_i) != _origins.end();
		if (!exists_column) { return false; }
		bool exists_square = _origins.find(origin_i)->second.find(origin_j) != 
		                     _origins.find(origin_i)->second.end();
		if (!exists_square) { return false; }
		return _origins[origin_i][origin_j].exists_destin(destin);
	}

	odx_matrix* odx_matrix::_instance = nullptr;

} // namespace urban

#endif