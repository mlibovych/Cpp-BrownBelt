#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <set>
#include <typeinfo>

namespace rng = std::ranges;
namespace view = rng::views;

template <rng::input_range Range>
void Print(const Range& range) {
    std::cout << "range print" << std::endl;
    for (const auto& x : range) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void Print(const T& x) {
    std::cout << x << std::endl;
}

// int main() {
//     std::vector v = {4, 1, 7, 2, 3, 8};
//     rng::sort(v);
//     Print(v);
    
//     return 0;
// }

// int main() {
//     std::vector<int> numbers = {1, 5, 6, 7, 7, 8};

//     for (int i : view::iota(0, 5)) {
//         std::cout << numbers[i] << ' ';
//     }
        // for (int x : numbers | view::take(5)) {
        //     std::cout << x << ' ';
        // }
    
//     return 0;
// }

void GetParts() {
    std::vector<int> parts = {4, 1, 7, 2, 3, 8};

    for (int x : parts | view::take(5)) {
        std::cout << x << ' ';
    }
}

int main() {
    std::set<int> parts = {4, 1, 7, 2, 3, 8};

    auto a = view::all(parts);
    Print(a);

    rng::take_view b = parts | view::take(4);
    Print(b);

    rng::ref_view c = view::all(parts);
    Print(c);

    return 0;
}

//g++-10 -std=c++20 test.cpp -o main
