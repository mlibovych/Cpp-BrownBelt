#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string_view>
#include <unordered_set>

using namespace std;

class Filter {
    unordered_set<string> banned;

    bool CheckDomain(string_view domain) {
        while (true) {
            size_t dot = domain.find('.');

            string subdomain = {domain.begin(), domain.end()};
            if (banned.count(subdomain)) {
                return 1;
            }
            if (dot == domain.npos) {
                break;
            }
            else {
                domain.remove_prefix(dot + 1);
            }
        }
        return 0;
    }

public:
    void ReadBannedDomains(istream &input = cin) {
        int count;

        for (input >> count; count > 0; --count) {
		    string domain;

            input >> domain;
            banned.insert(domain);
	    }
    }

    void CheckDomains(istream &input = cin, ostream &output = cout) {
        int count;

        for (input >> count; count > 0; --count) {
		    string domain;

            input >> domain;
            if (CheckDomain(domain)) {
                output << "Bad\n";
            }
            else {
                output << "Good\n";
            }
	    }
    }
};

int main() {
    Filter filter;

    ios::sync_with_stdio(false);
	cin.tie(nullptr);
    filter.ReadBannedDomains(cin);
    filter.CheckDomains(cin, cout);
}