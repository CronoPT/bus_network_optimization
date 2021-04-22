#include <gtest/gtest.h>
#include <heap.hpp>

class heap_test: public testing::Test {  

	protected:
		net::heap<int> _heap = net::heap<int>();

};

TEST_F(heap_test, heap_indexes_test) {
	ASSERT_EQ(_heap.parent(1), 0);
	ASSERT_EQ(_heap.parent(2), 0);
	ASSERT_EQ(_heap.left(0), 1);
	ASSERT_EQ(_heap.right(0), 2);

	ASSERT_EQ(_heap.parent(5), 2);
	ASSERT_EQ(_heap.parent(6), 2);
	ASSERT_EQ(_heap.left(2), 5);
	ASSERT_EQ(_heap.right(2), 6);

	ASSERT_EQ(_heap.parent(7), 3);
	ASSERT_EQ(_heap.parent(8), 3);
	ASSERT_EQ(_heap.left(3), 7);
	ASSERT_EQ(_heap.right(3), 8);
}

TEST_F(heap_test, heap_ordering_test) {
	_heap.insert(14);
	_heap.insert(10);
	_heap.insert(1);
	_heap.insert(2);
	_heap.insert(4);
	_heap.insert(5);
	_heap.insert(11);

	ASSERT_EQ(_heap.extract_minimum(), 1);
	ASSERT_EQ(_heap.extract_minimum(), 2);
	ASSERT_EQ(_heap.extract_minimum(), 4);
	ASSERT_EQ(_heap.extract_minimum(), 5);
	ASSERT_EQ(_heap.extract_minimum(), 10);
	ASSERT_EQ(_heap.extract_minimum(), 11);
	ASSERT_EQ(_heap.extract_minimum(), 14);
}

TEST_F(heap_test, heap_update_test) {
	_heap.insert(14);
	_heap.insert(10);
	_heap.insert(1);
	_heap.insert(2);
	_heap.insert(4);
	_heap.insert(5);
	_heap.insert(11);

	_heap.decrease_key(14, 7);
	_heap.decrease_key(11, 6);

	ASSERT_EQ(_heap.extract_minimum(), 1);
	ASSERT_EQ(_heap.extract_minimum(), 2);
	ASSERT_EQ(_heap.extract_minimum(), 4);
	ASSERT_EQ(_heap.extract_minimum(), 5);
	ASSERT_EQ(_heap.extract_minimum(), 6);
	ASSERT_EQ(_heap.extract_minimum(), 7);
	ASSERT_EQ(_heap.extract_minimum(), 10);
}

TEST_F(heap_test, heap_contains_test) {

	ASSERT_TRUE(_heap.size()==0);

	_heap.insert(14);
	_heap.insert(10);
	_heap.insert(1);
	_heap.insert(2);
	_heap.insert(4);
	_heap.insert(5);
	_heap.insert(11);

	ASSERT_TRUE(_heap.size()==7);

	ASSERT_TRUE(_heap.contains(1));
	ASSERT_TRUE(_heap.contains(2));
	ASSERT_TRUE(_heap.contains(4));
	ASSERT_TRUE(_heap.contains(5));
	ASSERT_TRUE(_heap.contains(10));
	ASSERT_TRUE(_heap.contains(14));
	ASSERT_TRUE(_heap.contains(11));
}