#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class RouteManager {
    map<int, set<int>> reachable_lists_;
public:
    void AddRoute(int start, int finish) {
        reachable_lists_[start].insert(finish);
        reachable_lists_[finish].insert(start);
    }
    int FindNearestFinish(int start, int finish) const {
        int result = abs(start - finish);
        if (reachable_lists_.count(start) < 1) {
            return result;
        }
        const set<int>& reachable_stations = reachable_lists_.at(start);
        const auto finish_pos = reachable_stations.lower_bound(finish);

        if (finish_pos != end(reachable_stations)) {
            result = min(result, abs(finish - *finish_pos));
        }
        if (finish_pos != begin(reachable_stations)) {
            result = min(result, abs(finish - *prev(finish_pos)));
        }
        return result;
    }
};


int main() {
	RouteManager routes;
	int query_count;

	for (cin >> query_count; query_count > 0; --query_count) {
		string query_type;
		int start, finish;

		cin >> query_type;
		cin >> start >> finish;
		if (query_type == "ADD") {
			routes.AddRoute(start, finish);
		} 
		else if (query_type == "GO") {
			cout << routes.FindNearestFinish(start, finish) << "\n";
		}
	}

	return 0;
}
