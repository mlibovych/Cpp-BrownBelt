#include "test_runner.h"
#include <limits>
#include <random>
#include <unordered_set>

using namespace std;

using CoordType = int;

struct Point3D {
    CoordType x;
    CoordType y;
    CoordType z;

    bool operator==(const Point3D& other) const {
        return tie(x, y, z) == tie(other.x, other.y, other.z);
    }
};

struct Hasher {
    size_t operator()(const Point3D &point) const {
        size_t r1 = chash(point.x);
        size_t r2 = chash(point.y);
        size_t r3 = chash(point.z);
        int x = 2123121;

        return r1 * x * x + r2 * x + r3;

    }
    std::hash<CoordType> chash;
};
