#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>

using namespace std;

class ReadingManager {
	static const int MAX_USER_COUNT_ = 100'000;
	static const int MAX_PAGE_COUNT_ = 1000;

	vector<int> page_users;
	map<int, int> user_pages;
public:
	ReadingManager() : page_users(MAX_PAGE_COUNT_, 0)
	{

	}

	void Read(int user_id, int page_count) {
		if (!user_pages.count(user_id)) {
			for (int i = page_count; i < MAX_PAGE_COUNT_; ++i) {
				page_users[i]++;
			}
		}
		else {
			for (int i = user_pages.at(user_id); i < page_count; ++i) {
				page_users[i]--;
			}
		}
		user_pages[user_id] = page_count;
	}

	double Cheer(int user_id) const {
		if (!user_pages.count(user_id)) {
			return 0;
		}
		if (user_pages.size() == 1) {
			return 1;
		}
		double users = page_users[user_pages.at(user_id) - 1];

		return users / (user_pages.size() - 1);
  	}
};


int main() {
	ReadingManager manager;
	int query_count;

	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	for (cin >> query_count; query_count > 0; --query_count) {
		string query_type;
		int user_id;

		cin >> query_type;
		cin >> user_id;
		if (query_type == "READ") {
			int page_count;

			cin >> page_count;
			manager.Read(user_id, page_count);
		}
		else if (query_type == "CHEER") {
			cout << setprecision(6) << manager.Cheer(user_id) << "\n";
		}
	}

	return 0;
}