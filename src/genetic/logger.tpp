#ifndef __LOGGER_CPP__
#define __LOGGER_CPP__

namespace genetic {
	
	logger::logger():
	 _summary(),
	 _curr_iteration(),
	 _pop_size(0),
	 _mutation_prob(0.0),
	 _crossover_prob(0.0) {
		/* Do Nothing */
	}

	logger* logger::instance() {
		if (!_instance) {
			_instance = new logger();
		}
		return _instance;
	}

	logger* logger::_instance = nullptr;

	void logger::end_iteration() {
		_summary.push_back(_curr_iteration);
		_curr_iteration = std::vector<std::vector<float>>();
	}

	void logger::add_individual(std::vector<float> individual) {
		_curr_iteration.push_back(individual);
	}

	void logger::mark_pop_size(int size) {
		_pop_size = size;
	}

	void logger::mark_mutation_prob(float prob) {
		_mutation_prob = prob;
	}

	void logger::mark_crossover_prob(float prob) {
		_crossover_prob = prob;
	}

	void logger::persist() {
		std::ofstream file("../data/json/run.json");

		file << "{\n";

		file << "\t\"population_size\": " << _pop_size << ",\n";
		file << "\t\"mutation_probability\": "  << _mutation_prob  << ",\n";
		file << "\t\"crossover_probability\": " << _crossover_prob << ",\n";
		file << "\t\"summary\": [\n";
		for (int iteration_index=0; iteration_index<_summary.size(); iteration_index++) {
			auto& iteration = _summary.at(iteration_index);
			file << "\t\t[\n";

			for (int j=0; j<iteration.size(); j++) {
				auto& individual = iteration.at(j);
				file << "\t\t\t[";
				for (int i=0; i<individual.size(); i++) {
					file << individual.at(i);
					if (i<individual.size()-1) { file << ", "; }
				}
				if (j < iteration.size()-1) {
					file << "],\n";
				} else { file << "]\n"; }
				
			}

			if (iteration_index < _summary.size()-1) {
				file << "\t\t],\n";
			} else { file << "\t\t]\n"; }
		
		}
		file << "\t]\n";
		file << "}";

		file.close();
	}
}

#endif