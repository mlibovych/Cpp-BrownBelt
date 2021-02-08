#include "json.h"
#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

struct Spending {
  string category;
  int amount;
};

bool operator == (const Spending& lhs, const Spending& rhs) {
  return lhs.category == rhs.category && lhs.amount == rhs.amount;
}

ostream& operator << (ostream& os, const Spending& s) {
  return os << '(' << s.category << ": " << s.amount << ')';
}

int CalculateTotalSpendings(
  const vector<Spending>& spendings
) {
  int result = 0;
  for (const Spending& s : spendings) {
    result += s.amount;
  }
  return result;
}

string MostExpensiveCategory(
  const vector<Spending>& spendings
) {
  auto compare_by_amount =
    [](const Spending& lhs, const Spending& rhs) {
      return lhs.amount < rhs.amount;
    };
  return max_element(begin(spendings), end(spendings),
    compare_by_amount)->category;
}

vector<Spending> LoadFromJson(istream& input) {
	Document doc = Load(input);
	vector<Spending> res;

	res.reserve(doc.GetRoot().AsArray().size());
	for (Node node : doc.GetRoot().AsArray()) {
		res.push_back({
			node.AsMap().at("category").AsString(),
			node.AsMap().at("amount").AsInt()
		});
	}
	return res;
}
