#ifndef __PRIORITY_QUEUE_HPP__
#define __PRIORITY_QUEUE_HPP__

#include <set>

namespace net {

	template<typename T>
	class priority_queue {
		class item {
			public:
				float _cost;
				T _key;
				item(T key, float cost);

		};

		std::set<item> _set;

		public:
			priority_queue();
			void push(T key, float cost);
			T pop();
			void update(T key, float new_cost);
		
		friend bool operator<(const item& s1, const item& s2);
	};

} // namespace net

#include "priority_queue.tpp"

#endif