#include "test_runner.h"

#include <cstddef>  // нужно для nullptr_t
#include <utility>

using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
	T* m_ptr = nullptr;
public:
	UniquePtr() = default;
	UniquePtr(T * ptr) {
		m_ptr = ptr;
	}
	UniquePtr(const UniquePtr&) = delete;
	UniquePtr(UniquePtr&& other) {
		m_ptr = other.Release();
	}
	UniquePtr& operator = (const UniquePtr&) = delete;
	UniquePtr& operator = (nullptr_t) {
		Reset(nullptr);
		return *this;
	}
	UniquePtr& operator = (UniquePtr&& other) {
		if (this != &other) {
			Reset(other.m_ptr);
			other.Reset(nullptr);
		}
		return *this;
	}
	~UniquePtr() {
		delete m_ptr;
	}

	T& operator * () const {
		return *m_ptr;
	}

	T * operator -> () const {
		return m_ptr;
	}

	T *Release() {
		T *res = m_ptr;

		m_ptr = nullptr;
		return res;
	}

	void Reset(T * ptr) {
		delete m_ptr;
		m_ptr = ptr;
	}

	void Swap(UniquePtr& other) {
		swap(m_ptr, other.m_ptr);
	}

	T *Get() const {
		return m_ptr;
	}
};


struct Item {
	static int counter;
	int value;
	Item(int v = 0): value(v) {
		++counter;
	}
	Item(const Item& other): value(other.value) {
		++counter;
	}
	~Item() {
		--counter;
	}
};

int Item::counter = 0;


void TestLifetime() {
	Item::counter = 0;
	{
		UniquePtr<Item> ptr(new Item);
		ASSERT_EQUAL(Item::counter, 1);

		ptr.Reset(new Item);
		ASSERT_EQUAL(Item::counter, 1);
	}
	ASSERT_EQUAL(Item::counter, 0);

	{
		UniquePtr<Item> ptr(new Item);
		ASSERT_EQUAL(Item::counter, 1);

		auto rawPtr = ptr.Release();
		ASSERT_EQUAL(Item::counter, 1);

		delete rawPtr;
		ASSERT_EQUAL(Item::counter, 0);
	}
	ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
	UniquePtr<Item> ptr(new Item(42));
	ASSERT_EQUAL(ptr.Get()->value, 42);
	ASSERT_EQUAL((*ptr).value, 42);
	ASSERT_EQUAL(ptr->value, 42);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestLifetime);
	RUN_TEST(tr, TestGetters);
	
	// UniquePtr<Item> ptr1(nullptr);
	// UniquePtr<Item> ptr2(new Item(42));
	// ptr2 = nullptr;
	// ptr1 = nullptr;
	// ptr1.Swap(ptr2);
	cout << "here" << endl;
}

