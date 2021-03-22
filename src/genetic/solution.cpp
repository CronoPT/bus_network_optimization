
#include "solution.hpp"

template<typename T>
genetic::solution<T>::solution(const T item):
 _total_cost(-1), _fitness(-1), _costs(0, 0), _item(item) {

}

template<typename T>
float genetic::solution<T>::total_cost() const {
	return _total_cost;
}

template<typename T>
float genetic::solution<T>::fitness() const {
	return _fitness;
}

template<typename T>
const std::vector<float>& genetic::solution<T>::costs() const {
	return _costs;
}

template<typename T>
const T& genetic::solution<T>::item() const {
	return _item;
}

template<typename T>
void genetic::solution<T>::total_cost(float new_total) {
	_total_cost = new_total;
}

template<typename T>
void genetic::solution<T>::fitness(float new_fitness) {
	_fitness = new_fitness;
}

template<typename T>
void genetic::solution<T>::costs(const std::vector<float>& new_costs) {
	_costs = new_costs;
}

template<typename T>
void genetic::solution<T>::item(const T& new_item) {
	_item = new_item;
}

template<typename T>
bool genetic::solution<T>::operator<(const solution<T>& other) const {
	return _fitness < other.fitness();
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const genetic::solution<T>& s) {
	s.dump_to(os);
	return os;
}

template class genetic::solution<int>;