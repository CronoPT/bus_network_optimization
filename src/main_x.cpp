#include <solution.hpp>
#include <problem.hpp>
#include <classic_ga.hpp>
#include <constraint.hpp>
#include <cost_function.hpp>
#include <osm_net.hpp>
#include <edge.hpp>
#include <node.hpp>
#include <graph.hpp>
#include <nsga.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <ctime>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <bitset>

class x_bin {

	int _x;
	int _max;
	int _min;

	public:
	x_bin() {

	}

	x_bin(int int_x, int max, int min) {
		_x   = int_x;
		_max = max;
		_min = min;
	}

	x_bin(float flt_x, int max, int min) {
		_x = (flt_x * std::numeric_limits<int>::max()) / 
		     ((float) (max-min)) - min;

		_max = max;
		_min = min; 
	}

	int get_int_x() const {
		return _x;
	}

	void set_int_x(int new_x) {
		_x = new_x;
	}

	float get_x() const {
		return ((float) _x / (float) std::numeric_limits<int>::max()) *
		       ((float) (_max-_min)) + _min; 
	}

};


std::ostream& operator<<(std::ostream& os, const x_bin& c) {
	os << std::fixed;
	os << std::setprecision(2);
	os << "{x: " << c.get_x() << ", x_int: "
	   << c.get_int_x() << "}";
	return os;
}


class x_squared: public genetic::cost_function<x_bin> {
	public:
		float compute(x_bin& bin) const {
			return std::pow(bin.get_x(), 2);
		}
};

class one_over_x: public genetic::cost_function<x_bin> {
	public:
		float compute(x_bin& bin) const {
			return 1/bin.get_x();
			// return std::sin(bin.get_x());
		}
};


class x_constraint: public genetic::constraint<x_bin> {
	public:
		bool satisfied(x_bin& bin) const {
			return bin.get_x()>0 && bin.get_x()<10;
		}

		float transgression(x_bin& bin) const {
			if (satisfied(bin)) {
				return 0.0;
			} else if (bin.get_x() >= 10) {
				return std::pow((bin.get_x()-10)+2, 2);
			} else if (bin.get_x() <= 0) {
				return std::pow((0-bin.get_x())+2, 2);
			}
			return 0.0;
		}
};

class x_problem: genetic::problem<x_bin> {
	public:
		x_problem(genetic::constraint_set<x_bin> constraints, 
		 genetic::cost_function_set<x_bin> cost_functions):
		 problem(constraints, cost_functions) {

		}

		std::vector<genetic::solution<x_bin>> initialize_population() override {
			auto pop = std::vector<genetic::solution<x_bin>>();
			for (int i=0; i<20; i++) {
				pop.push_back(genetic::solution<x_bin>(x_bin(
					std::rand(), 10, 1
				)));
			}

			return pop;
		}

		std::pair<x_bin, x_bin> crossover(x_bin& x1, x_bin& x2) override {

			float crossover_percent = ((float) std::rand() / RAND_MAX);
			int cro_pt = 31 * crossover_percent;
			int max = std::numeric_limits<int>::max();

			int new_x1_int = (x1.get_int_x() & ((max << cro_pt) & max)) +
			                 (x2.get_int_x() & ((max >> (31-cro_pt)) & max));
			int new_x2_int = (x2.get_int_x() & ((max << cro_pt) & max)) +
			                 (x1.get_int_x() & ((max >> (31-cro_pt)) & max));

			auto new_x1 = x_bin(new_x1_int, 10, 1);
			auto new_x2 = x_bin(new_x2_int, 10, 1);
			
			std::cout << "<<Crossover>>" << std::endl;
			std::cout << std::bitset<32>(x1.get_int_x()) << std::endl;
			std::cout << std::bitset<32>(x2.get_int_x()) << std::endl;
			std::cout << std::bitset<32>(new_x1_int) << std::endl;
			std::cout << std::bitset<32>(new_x2_int) << std::endl;
			std::cout << std::bitset<32>(((max << cro_pt) & max)) << std::endl;
			std::cout << std::bitset<32>(((max >> (31-cro_pt)) & max)) << std::endl;

			return std::pair<x_bin, x_bin>(new_x1, new_x2);
		}

		void mutate(x_bin& x) const override {
			std::cout << "<<Mutation>>" << std::endl;
			float crossover_percent = ((float) std::rand() / RAND_MAX);
			std::cout << std::bitset<32>(x.get_int_x()) << std::endl;
			int cro_pt = 31 * crossover_percent;
			int int_x  = x.get_int_x();
			auto b_set = std::bitset<32>(int_x);
			b_set.flip(cro_pt);
			int new_x_int = (int) b_set.to_ulong();
			x.set_int_x(new_x_int);
			std::cout << std::bitset<32>(x.get_int_x()) << std::endl;
		}
};

int main() {

	std::srand(static_cast<unsigned int>(std::time(nullptr))); 

	auto constraints = genetic::constraint_set<x_bin>();
	constraints.push_back(std::make_shared<x_constraint>(x_constraint()));

	auto cost_functs = genetic::cost_function_set<x_bin>();
	cost_functs.push_back(std::make_shared<x_squared>(x_squared()));
	cost_functs.push_back(std::make_shared<one_over_x>(one_over_x()));

	auto problem = new x_problem(constraints, cost_functs);

	auto GA = new genetic::nsga<x_bin>((genetic::problem<x_bin>*) problem);

	auto solutions = GA->execute(1000, 0.0001, 30);
	
	std::cout << "Best Solution(s):";
	int count = 1;
	for (auto solution: solutions) {
		std::cout << "\n\t" << count++ << ": "
		          << solution.get_item()
		          << "\t-- cost: " << solution.get_total_cost()
		          << "\t-- costs: ";
		for (auto cost: solution.get_costs()) {
			std::cout << cost << " ";
		}

		std::cout << " -- trans: ";
		for (auto tran: solution.get_transgressions()) {
			std::cout << tran << " ";
		}

		std::cout << " -- total_trans: " << solution.get_total_transgression();
	}
	std::cout << std::endl;

	auto n1 = x_bin(0, 10, 1);
	auto n2 = x_bin(std::numeric_limits<int>::max(), 10, 1);

	std::cout << n1 << std::endl;
	std::cout << n2 << std::endl;


	delete GA;
	delete problem;

	// std::cout << std::numeric_limits<int>::max() << std::endl;
	// std::cout << std::numeric_limits<int>::min() << std::endl;
	// std::cout << RAND_MAX << std::endl;
	// std::cout << "Unsigned: " << std::bitset<32>(std::numeric_limits<unsigned int>::max()) << std::endl;
	// std::cout << "Signed:   " << std::bitset<32>(std::numeric_limits<int>::max()) << std::endl;
	// std::cout << "Signed:   " << std::bitset<32>(std::numeric_limits<int>::min()) << std::endl;

	// int a = (std::numeric_limits<int>::max()<<31);
	// std::cout << "Var: " << a << std::endl;
	// std::cout << "Bin var: " << std::bitset<32>(a) <<std::endl;

}