#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
#include <deque>

// #include "test_runner.h"

using namespace std;

template <class T>
class ObjectPool {
    set<T*> allocated;
    deque<T*> freed;
public:
    T* Allocate();

    T* TryAllocate();

    void Deallocate(T* object) {
        if (!allocated.count(object))
            throw invalid_argument("oooo");
        freed.push_back(object);
        allocated.erase(object);
    }

    ~ObjectPool() {
        for (auto& it : allocated) {
            delete it;
        }
        for (auto& it : freed) {
            delete it;
        }
    }
};

template <typename T>
T* ObjectPool<T>::Allocate() {
    T* obj;

    if (!(freed).empty()) {
        obj = freed.front();
        freed.pop_front();
    }
    else {
        obj = new T;
    }
    allocated.insert(obj);
    return obj;
}

template <typename T>
T* ObjectPool<T>::TryAllocate() {
    if (!(freed).empty()) {
        return Allocate();
    }
    else {
        return nullptr;
    }
}



// void TestObjectPool() {
//   ObjectPool<string> pool;

//   auto p1 = pool.Allocate();
//   auto p2 = pool.Allocate();
//   auto p3 = pool.Allocate();

//   *p1 = "first";
//   *p2 = "second";
//   *p3 = "third";

//   pool.Deallocate(p2);
//   ASSERT_EQUAL(*pool.Allocate(), "second");

//   pool.Deallocate(p3);
//   pool.Deallocate(p1);
//   ASSERT_EQUAL(*pool.Allocate(), "third");
//   ASSERT_EQUAL(*pool.Allocate(), "first");

//   pool.Deallocate(p1);
// }

// int main() {
//   TestRunner tr;
//   RUN_TEST(tr, TestObjectPool);
//   return 0;
// }
