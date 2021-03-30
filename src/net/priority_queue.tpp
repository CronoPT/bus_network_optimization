#ifndef __PRIORITY_QUEUE_TPP__
#define __PRIORITY_QUEUE_TPP__

namespace net {

	template<typename T>
	bool operator<(const priority_queue<T>::item& s1, const priority_queue<T>::item& s2) {
		return s1.cost < s2.cost;
	}

	template<typename T>
	priority_queue<T>::item::item(T key, float cost):
	 _cost(cost), _key(T) {

	}

	template<typename T>
	priority_queue<T>::priority_queue():
	 _set() {

	}

	template<typename T>
	void priority_queue<T>::push(T key, float cost) {
		auto holder = item(key, cost);
		_set.insert(item);
	}

	template<typename T>
	T priority_queue<T>::pop() {
		auto holder = *_set.begin();
		_set.erase(holder);
		return holder._key;
	}

	template<typename T>
	void priority_queue<T>::update(T key, float new_cost) {
		for (auto holder: _set) {
			auto it = std::find_if(_set.begin(), _set.end(), [](item holder){
				return holder._key == key;
			})
		}
	}



} // namespace net

#endif