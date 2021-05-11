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

class bin_float {

	int _x;
	int _max;
	int _min;

	public:
	bin_float() {

	}

	bin_float(int int_x, int max, int min) {
		_x   = int_x;
		_max = max;
		_min = min;
	}

	bin_float(float flt_x, int max, int min) {
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

	std::pair<bin_float, bin_float> crossover(const bin_float& other) const {
		float crossover_percent = ((float) std::rand() / RAND_MAX);
		int cro_pt = 31 * crossover_percent;
		int max = std::numeric_limits<int>::max();



		int x1_int = (get_int_x() & ((max << cro_pt) & max)) +
						(other.get_int_x() & ((max >> (31-cro_pt)) & max));
		int x2_int = (other.get_int_x() & ((max << cro_pt) & max)) +
						(get_int_x() & ((max >> (31-cro_pt)) & max));

		auto x1 = bin_float(x1_int, _max, _min);
		auto x2 = bin_float(x2_int, _max, _min);

		return std::pair<bin_float, bin_float>(x1, x2);
	}

	void mutate() {
		float crossover_percent = ((float) std::rand() / RAND_MAX);
		int cro_pt = 31 * crossover_percent;
		int int_x  = get_int_x();
		auto b_set = std::bitset<32>(int_x);
		b_set.flip(cro_pt);
		int new_x_int = (int) b_set.to_ulong();
		_x = new_x_int;
	}

};

std::ostream& operator<<(std::ostream& os, const bin_float& c) {
	os << std::fixed;
	os << std::setprecision(2);
	os << c.get_x();
	return os;
}

class ciara {
	bin_float _var1;
	bin_float _var2;

	public:
	ciara() { /*Do Nothing*/ }
	ciara(bin_float var1, bin_float var2): 
	 _var1(var1), _var2(var2) {

	}

	const bin_float& get_var1() const { return _var1; }
	const bin_float& get_var2() const { return _var2; }

	friend std::ostream& operator<<(std::ostream& os, const ciara& c) {
		os << "{var1: " << c._var1 << ", var2: "
		   << c._var2 << "}";
		return os;
	} 

	void mutate_var1() {
		_var1.mutate();
	}

	void mutate_var2() {
		_var2.mutate();
	}

};

class objective_1: public genetic::cost_function<ciara> {
	public:
	float compute(ciara& sol) const {
		return 3*sol.get_var1().get_x() + sol.get_var2().get_x();
	}
};

class objective_2: public genetic::cost_function<ciara> {
	public:
	float compute(ciara& sol) const {
		return -sol.get_var1().get_x() + 4*sol.get_var2().get_x();
	}
};

class constraint_1: public genetic::constraint<ciara> {
	public:
	float val(ciara& sol) const {
		return sol.get_var2().get_x();
	}

	bool satisfied(ciara& sol) const {
		return val(sol)<=6;
	}

	float transgression(ciara& sol) const {
		if (satisfied(sol)) {
			return 0.0;
		} else {
			return std::pow((val(sol)-6)+2, 2);
		}
	}
};

class constraint_2: public genetic::constraint<ciara> {
	public:
	float val(ciara& sol) const {
		return sol.get_var1().get_x() + 3*sol.get_var2().get_x();
	}

	bool satisfied(ciara& sol) const {
		return val(sol)>=3;
	}

	float transgression(ciara& sol) const {
		if (satisfied(sol)) {
			return 0.0;
		} else {
			return std::pow((3-val(sol))+2, 2);
		}
	}
};

class constraint_3: public genetic::constraint<ciara> {
	public:
	float val(ciara& sol) const {
		return 2*sol.get_var1().get_x() - sol.get_var2().get_x();
	}

	bool satisfied(ciara& sol) const {
		return val(sol)<=6;
	}

	float transgression(ciara& sol) const {
		if (satisfied(sol)) {
			return 0.0;
		} else {
			return std::pow((val(sol)-6)+2, 2);
		}
	}
};

class constraint_4: public genetic::constraint<ciara> {
	public:
	float val(ciara& sol) const {
		return 2*sol.get_var1().get_x() + sol.get_var2().get_x();
	}

	bool satisfied(ciara& sol) const {
		return val(sol)<=10;
	}

	float transgression(ciara& sol) const {
		if (satisfied(sol)) {
			return 0.0;
		} else {
			return std::pow((val(sol)-10)+2, 2);
		}
	}
};

class constraint_5: public genetic::constraint<ciara> {
	public:
	float val(ciara& sol) const {
		return sol.get_var1().get_x();
	}

	bool satisfied(ciara& sol) const {
		return val(sol)>=0;
	}

	float transgression(ciara& sol) const {
		if (satisfied(sol)) {
			return 0.0;
		} else {
			return std::pow((0-val(sol))+2, 2);
		}
	}
};

class constraint_6: public genetic::constraint<ciara> {
	public:
	float val(ciara& sol) const {
		return sol.get_var2().get_x();
	}

	bool satisfied(ciara& sol) const {
		return val(sol)>=0;
	}

	float transgression(ciara& sol) const {
		if (satisfied(sol)) {
			return 0.0;
		} else {
			return std::pow((0-val(sol))+2, 2);
		}
	}
};

class ciara_problem: genetic::problem<ciara> {
	public:
		ciara_problem(genetic::constraint_set<ciara> constraints, 
		 genetic::cost_function_set<ciara> cost_functions):
		 problem(constraints, cost_functions) {

		}

		std::vector<genetic::solution<ciara>> initialize_population() override {
			auto pop = std::vector<genetic::solution<ciara>>();
			for (int i=0; i<60; i++) {
				pop.push_back(genetic::solution<ciara>(ciara(
					bin_float(std::rand(), 10, 0),
					bin_float(std::rand(), 10, 0)
				)));
			}

			return pop;
		}

		std::pair<ciara, ciara> crossover(ciara& c1, ciara& c2) override {

			auto vars1 = c1.get_var1().crossover(c2.get_var1());
			auto vars2 = c1.get_var2().crossover(c2.get_var2());

			auto new_c1 = ciara(vars1.first, vars2.first);
			auto new_c2 = ciara(vars1.second, vars2.second);

			return std::pair<ciara, ciara>(new_c1, new_c2);
		}

		void mutate(ciara& c) const override {
			c.mutate_var1();
			c.mutate_var2();
		}
};

int main() {

	std::cout << std::fixed;
	std::cout << std::setprecision(2);

	std::srand(static_cast<unsigned int>(std::time(nullptr))); 

	auto constraints = genetic::constraint_set<ciara>();
	constraints.push_back(std::make_shared<constraint_1>(constraint_1()));
	constraints.push_back(std::make_shared<constraint_2>(constraint_2()));
	constraints.push_back(std::make_shared<constraint_3>(constraint_3()));
	constraints.push_back(std::make_shared<constraint_4>(constraint_4()));
	constraints.push_back(std::make_shared<constraint_5>(constraint_5()));
	constraints.push_back(std::make_shared<constraint_6>(constraint_6()));

	auto cost_functs = genetic::cost_function_set<ciara>();
	cost_functs.push_back(std::make_shared<objective_1>(objective_1()));
	cost_functs.push_back(std::make_shared<objective_2>(objective_2()));

	auto problem = new ciara_problem(constraints, cost_functs);

	auto GA = new genetic::nsga<ciara>((genetic::problem<ciara>*) problem);

	auto solutions = GA->execute(1000, 0.0001, 30);
	
	std::cout << "Best Solution(s):";
	int count = 1;
	for (auto solution: solutions) {
		std::cout << "\n\t" << std::setfill('0')
		          << std::setw(2)
		          << count++ << ": "
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

	auto n1 = bin_float(0, 10, 1);
	auto n2 = bin_float(std::numeric_limits<int>::max(), 10, 1);

	std::cout << n1 << std::endl;
	std::cout << n2 << std::endl;


	delete GA;
	delete problem;

}