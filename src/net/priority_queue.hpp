#ifndef __PRIORITY_QUEUE_HPP__
#define __PRIORITY_QUEUE_HPP__

#include <set>
#include <iostream>
#include <algorithm>
#include "heap.hpp"

namespace net {

	template<typename T>
	class item {
		public:
			float _cost;
			T _key;
			item(T key, float cost);

		template<typename C>
		friend bool operator<(const item<C>& i1, const item<C>& i2);
		template<typename C>
		friend bool operator>(const item<C>& i1, const item<C>& i2);
		template<typename C>
		friend bool operator==(const item<C>& i1, const item<C>& i2);
	};

	template<typename T>
	class priority_queue {

		private:
			heap<item<T>> _heap;

		public:
			priority_queue();
			void push(T key, float cost);
			T pop();
			void update(T key, float new_cost);
			int  size();
			bool empty();
			bool contains(T key);
			void print();

	};

} // namespace net

namespace std {
	template<typename T>
	struct hash<net::item<T>> {
		std::size_t operator()(const net::item<T>& item) const {
			return hash<int>()(item._key);
		}
	};
}

#include "priority_queue.tpp"

#endif