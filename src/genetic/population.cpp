#include "population.hpp"

template<typename T>
genetic::population<T>::population(std::vector<solution<T>>& sols):
 _solutions(sols){

}
