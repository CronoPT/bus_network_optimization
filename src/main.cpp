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

class cone {

	private:
		float _radius;
		float _height;

	public:
		cone():
		 _radius(0), _height(0) {

		}

		cone(float radius, float height): 
		 _radius(radius), _height(height) {

		}

		float radius() const { return _radius; }
		float height() const { return _height; }

		void radius(float radius) { _radius = radius; }
		void height(float height) { _height = height; }

		float volume() const {
			return (3.14/3) * std::pow(_radius, 2) * _height;
		}

		float lateral_area() const {
			float lateral_height = std::sqrt(
				std::pow(_radius, 2) +
				std::pow(_height, 2)
			);
			float lateral =  3.14 * _radius * lateral_height;

			return lateral;
		}

		float base_area() const {
			float base = 3.14 * std::pow(_radius, 2); 

			return base;
		}

		float total_area() const {
			return base_area() + lateral_area();
		}

};

std::ostream& operator<<(std::ostream& os, const cone& c) {
	os << std::fixed;
	os << std::setprecision(2);
	os << "{radius: " << c.radius() << ", height: "
	   << c.height() << ", volume: " << c.volume() 
	   << ", lateral_area: " << c.lateral_area() 
	   << ", total_area: " << c.total_area() << "}";
	return os;
}

class min_total_surface: public genetic::cost_function<cone> {
	public:
		float compute(const cone& sol) const {
			return sol.total_area();
		}
};

class min_latetal_surface: public genetic::cost_function<cone> {
	public:
		float compute(const cone& sol) const {
			return sol.lateral_area();
		}
};


class volume_constraint: public genetic::constraint<cone> {
	public:
		bool satisfied(const cone& sol) const {
			return sol.volume() > 200;
		}

		float transgression(const cone& sol) const {
			if (satisfied(sol)) {
				return 0.0;
			} else {
				return std::pow((200 - sol.volume())+10, 2);
			}
		}
};

class radius_constraint: public genetic::constraint<cone> {
	public:
		bool satisfied(const cone& sol) const {
			return sol.radius() <= 10;
		}

		float transgression(const cone& sol) const {
			if (satisfied(sol)) {
				return 0.0;
			} else {
				return std::pow((sol.radius() - 10)+2, 2);
			}
		}
};

class height_constraint: public genetic::constraint<cone> {
	public:
		bool satisfied(const cone& sol) const {
			return sol.height() <= 20;
		}

		float transgression(const cone& sol) const {
			if (satisfied(sol)) {
				return 0.0;
			} else {
				return std::pow((sol.height() - 20)+2, 2);
			}
		}
};

class cones_problem: genetic::problem<cone> {
	public:
		cones_problem(genetic::constraint_set<cone> constraints, 
		 genetic::cost_function_set<cone> cost_functions):
		 problem(constraints, cost_functions) {

		}

		std::vector<genetic::solution<cone>> initialize_population() override {
			auto pop = std::vector<genetic::solution<cone>>();
			pop.push_back(genetic::solution<cone>(cone(1, 2)));
			pop.push_back(genetic::solution<cone>(cone(2, 1)));
			pop.push_back(genetic::solution<cone>(cone(1, 4)));
			pop.push_back(genetic::solution<cone>(cone(4, 2)));
			pop.push_back(genetic::solution<cone>(cone(3, 3)));
			pop.push_back(genetic::solution<cone>(cone(9, 8)));
			pop.push_back(genetic::solution<cone>(cone(6, 4)));
			pop.push_back(genetic::solution<cone>(cone(8, 3)));
			pop.push_back(genetic::solution<cone>(cone(8, 15)));
			pop.push_back(genetic::solution<cone>(cone(7, 18)));
			pop.push_back(genetic::solution<cone>(cone(1, 19)));
			pop.push_back(genetic::solution<cone>(cone(9.9, 19.9)));
			return pop;
		}

		std::pair<cone, cone> crossover(cone& i1, cone& i2) override {
			cone cone1 = cone(i1.radius(), i2.height());
			cone cone2 = cone(i2.radius(), i1.height());
			return std::pair<cone, cone>(cone1, cone2);

		}

		void mutate(cone& item) const override {
			bool mutate_radius = ((float) std::rand() / RAND_MAX) > 0.5;
			bool mutate_height = ((float) std::rand() / RAND_MAX) > 0.5;
			
			if (mutate_radius) {
				bool subtract = ((float) std::rand() / RAND_MAX) > 0.5;
				if (subtract && item.radius()>0.1) {
					item.radius(item.radius()-0.1);
				} else {
					item.radius(item.radius()+0.1);
				}
			}

			if (mutate_height) {
				bool subtract = ((float) std::rand() / RAND_MAX) > 0.5;
				if (subtract && item.height()>0.1) {
					item.height(item.height()-0.1);
				} else {
					item.height(item.height()+0.1);
				}
			}
		}
};

int main() {

	auto g = osm_net::import_network();

	std::srand(static_cast<unsigned int>(std::time(nullptr))); 

	auto constraints = genetic::constraint_set<cone>();
	constraints.push_back(std::make_shared<volume_constraint>(volume_constraint()));
	constraints.push_back(std::make_shared<radius_constraint>(radius_constraint()));
	constraints.push_back(std::make_shared<height_constraint>(height_constraint()));

	auto cost_functs = genetic::cost_function_set<cone>();
	cost_functs.push_back(std::make_shared<min_total_surface>(min_total_surface()));
	cost_functs.push_back(std::make_shared<min_latetal_surface>(min_latetal_surface()));

	auto problem = new cones_problem(constraints, cost_functs);

	auto GA = new genetic::nsga<cone>((genetic::problem<cone>*) problem);

	auto solutions = GA->execute(1000, 0.0001, 30);
	
	std::cout << "Best Solution(s):";
	for (auto solution: solutions) {
		std::cout << "\n\t" << solution.get_item()
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

	std::cout << "Nodes: " << g.get_number_of_nodes()
	          << "\nEdges: " << g.get_number_of_edges()
			  << std::endl;

	delete GA;
	delete problem;
}