#include "xml.h"
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

vector<Spending> LoadFromXml(istream& input) {
	vector<Spending> res;
	Document doc = Load(input);

	res.reserve(doc.GetRoot().Children().size());
	for (Node node : doc.GetRoot().Children()) {
		res.push_back({node.AttributeValue<string>("category"), node.AttributeValue<int>("amount")});
	}
	return res;
}
