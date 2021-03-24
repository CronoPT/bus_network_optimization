#include <solution.hpp>
#include <problem.hpp>
#include <classic_ga.hpp>
#include <constraint.hpp>
#include <cost_function.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <memory>

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

};

std::ostream& operator<<(std::ostream& os, const cone& c) {
	os << "{radius: " << c.radius() << ", height: "
	   << c.height() << "}";
	return os;
}

class min_total_surface: public genetic::cost_function<cone> {
	public:
		float compute(const cone& sol) const {
			float base = 3.14 * std::pow(sol.radius(), 2); 
			float lateral_height = std::sqrt(
				std::pow(sol.radius(), 2) +
				std::pow(sol.height(), 2)
			);
			float lateral =  3.14 * sol.radius() * lateral_height;

			return base + lateral;
		}
};

class min_latetal_surface: public genetic::cost_function<cone> {
	public:
		float compute(const cone& sol) const {
			float lateral_height = std::sqrt(
				std::pow(sol.radius(), 2) +
				std::pow(sol.height(), 2)
			);
			float lateral =  3.14 * sol.radius() * lateral_height;

			return lateral;
		}
};


class volume_constraint: public genetic::constraint<cone> {
	public:
		bool satisfied(const cone& sol) const {
			float volume = (3.14/3) * std::pow(sol.radius(), 2) * sol.height();
			return volume > 200;
		}

		float transgression(const cone& sol) const {
			float volume = (3.14/3) * std::pow(sol.radius(), 2) * sol.height();

			if (satisfied(sol)) {
				return 0.0;
			} else {
				return 200 - volume;
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
				return sol.radius() - 10;
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
				return sol.height() - 20;
			}
		}
};

class cones_problem: genetic::problem<cone> {
	public:
		cones_problem(genetic::constraint_set<cone> constraints, 
		 genetic::cost_function_set<cone> cost_functions):
		 problem(constraints, cost_functions) {

		}

		std::vector<genetic::solution<cone>> initialize_population() const override {
			auto pop = std::vector<genetic::solution<cone>>();
			pop.push_back(genetic::solution<cone>(cone(1, 2)));
			pop.push_back(genetic::solution<cone>(cone(2, 1)));
			pop.push_back(genetic::solution<cone>(cone(1, 4)));
			pop.push_back(genetic::solution<cone>(cone(4, 2)));
			pop.push_back(genetic::solution<cone>(cone(3, 3)));
			pop.push_back(genetic::solution<cone>(cone(9, 8)));
			pop.push_back(genetic::solution<cone>(cone(6, 4)));
			pop.push_back(genetic::solution<cone>(cone(8, 3)));
			return pop;
		}

		std::pair<cone, cone> crossover(cone& i1, cone& i2) const override {
			cone cone1 = cone(i1.radius(), i2.height());
			cone cone2 = cone(i2.radius(), i1.height());
			return std::pair<cone, cone>(cone1, cone2);

		}

		void mutate(cone& item) const override {
			bool mutate_radius = ((float) std::rand() / RAND_MAX) > 0.5;
			bool mutate_height = ((float) std::rand() / RAND_MAX) > 0.5;
			
			if (mutate_radius) {
				bool subtract = ((float) std::rand() / RAND_MAX) > 0.5;
				if (subtract && item.radius()>1) {
					item.radius(item.radius()-1);
				} else {
					item.radius(item.radius()+1);
				}
			}

			if (mutate_height) {
				bool subtract = ((float) std::rand() / RAND_MAX) > 0.5;
				if (subtract && item.height()>1) {
					item.height(item.height()-1);
				} else {
					item.height(item.height()+1);
				}
			}
		}
};

int main() {
	auto constraints = genetic::constraint_set<cone>();
	constraints.push_back(std::make_shared<volume_constraint>(volume_constraint()));
	constraints.push_back(std::make_shared<radius_constraint>(radius_constraint()));
	constraints.push_back(std::make_shared<height_constraint>(height_constraint()));

	auto cost_functs = genetic::cost_function_set<cone>();
	cost_functs.push_back(std::make_shared<min_total_surface>(min_total_surface()));
	cost_functs.push_back(std::make_shared<min_latetal_surface>(min_latetal_surface()));

	auto problem = new cones_problem(constraints, cost_functs);

	auto GA = new genetic::classic_ga<cone>((genetic::problem<cone>*) problem);

	auto solution = GA->execute(100, 0.1);
	auto item = solution.item();

	std::cout << "Best cone: \n\tradius: "
		<< item.radius() << "\n\theight: " 
		<< item.height() << std::endl;

	delete GA;
	delete problem;
}