#ifndef __HEAP_HPP__
#define __HEAP_HPP__

#include <vector>
#include <unordered_map>

namespace net {

	template<typename T>
	class heap {
		std::vector<T> _vector;
		std::unordered_map<T, int> _indexes;
		int _heap_size;

		public:
		heap();
		int left(int index) const;
		int right(int index) const;
		int parent(int index) const;
		void min_heapify(int index);
		T extract_minimum();
		void insert(T element);
		void decrease_key_index(int index, T);
		void decrease_key(T old_element, T element);
	
	};

} // namespace heap

#include "heap.tpp"

#endif