#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <optional>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
        : first(begin)
        , last(end)
    {
    }

    Iterator begin() const {
        return first;
    }

    Iterator end() const {
        return last;
    }

private:
    Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
    return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person {
    string name;
    int age, income;
    bool is_male;
};

vector<Person> ReadPeople(istream& input) {
    int count;
    input >> count;

    vector<Person> result(count);
    for (Person& p : result) {
        char gender;
        input >> p.name >> p.age >> p.income >> gender;
        p.is_male = gender == 'M';
    }

    return result;
}

struct Stats {
    std::optional<string> MaleName;
    std::optional<string> FemaleName;
    vector<Person> people;
    vector<int> wealth;
};

template <typename Iterator>
std::optional<string> FindPopular(IteratorRange<Iterator> range) {
    if (range.begin() == range.end()) {
        return std::nullopt;
    }
    sort(range.begin(), range.end(), [](const Person& lhs, const Person& rhs) {
        return lhs.name < rhs.name;
    });
    const string* most_popular_name = &range.begin()->name;
    int count = 1;
    for (auto i = range.begin(); i != range.end(); ) {
        auto same_name_end = find_if_not(i, range.end(), [i](const Person& p) {
            return p.name == i->name;
        });
        auto cur_name_count = std::distance(i, same_name_end);
        if (cur_name_count > count) {
            count = cur_name_count;
            most_popular_name = &i->name;
        }
        i = same_name_end;
    }
    return *most_popular_name;
}

Stats ReadStats() {
    Stats res;

    res.people = ReadPeople(cin);
    auto it = partition(res.people.begin(), res.people.end(), [](const Person& person) {
        return person.is_male;
    });
    IteratorRange male(res.people.begin(), it);
    IteratorRange female(it, res.people.end());
    res.MaleName = FindPopular(male);
    res.FemaleName = FindPopular(female);

    sort(res.people.begin(), res.people.end(), [](const Person& lhs, const Person& rhs) {
        return lhs.income > rhs.income;
    });
    res.wealth.reserve(res.people.size());
    res.wealth[0] = res.people[0].income;
    for (int i = 1; i < res.people.size(); ++i) {
        res.wealth[i] = res.wealth[i - 1] + res.people[i].income;
    }

    sort(res.people.begin(), res.people.end(), [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
    });
    return res;
}

int main() {
    Stats stats = ReadStats();
    for (string command; cin >> command; ) {
        if (command == "AGE") {
        int adult_age;

        cin >> adult_age;
        auto adult_begin = lower_bound(
            begin(stats.people), end(stats.people), adult_age, [](const Person& lhs, int age) {
                return lhs.age < age;
            }
        );
        cout << "There are " << std::distance(adult_begin, end(stats.people))
            << " adult people for maturity age " << adult_age << '\n';
        } else if (command == "WEALTHY") {
            int count;

            cin >> count;
            cout << "Top-" << count << " people have total income " << stats.wealth[count - 1] << '\n';
        } else if (command == "POPULAR_NAME") {
            char gender;

            cin >> gender;
            std::optional<string> name = gender == 'M' ? stats.MaleName: stats.FemaleName;

            if (!name) {
                cout << "No people of gender " << gender << '\n';
            } else {
                cout << "Most popular name among people of gender " << gender << " is "
                    << *name << '\n';
            }
        }
    }
}