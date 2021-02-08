#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>
#include <ctime>
#include <string_view>
#include <map>
#include <stdexcept>
#include <iomanip>

using namespace std;

void LeftStrip(string_view& sv) {
    while (!sv.empty() && isspace(sv[0])) {
        sv.remove_prefix(1);
    }
}

string_view ReadToken(string_view& sv) {
    LeftStrip(sv);

    auto pos = sv.find('-');
    auto result = sv.substr(0, pos);
    sv.remove_prefix(pos != sv.npos ? pos + 1 : sv.size());
    return result;
}

class Date {
    friend istream& operator>>(istream &input, Date &date) {
        string s;
        string_view sv;

        input >> s;
        sv = s;
        date.year_ = stoi(string(ReadToken(sv)));
        date.month_ = stoi(string(ReadToken(sv)));
        date.day_ = stoi(string(ReadToken(sv)));
        return input;
    }
    friend bool operator<(const Date &lhs, const Date &rhs) {
        return tie(lhs.year_, lhs.month_, lhs.day_) < tie(rhs.year_, rhs.month_, rhs.day_);
    }
    friend bool operator<=(const Date &lhs, const Date &rhs) {
        return tie(lhs.year_, lhs.month_, lhs.day_) <= tie(rhs.year_, rhs.month_, rhs.day_);
    }
    friend bool operator>(const Date &lhs, const Date &rhs) {
        return tie(lhs.year_, lhs.month_, lhs.day_) > tie(rhs.year_, rhs.month_, rhs.day_);
    }

public:
    Date() = default;
    Date(int year, int month, int day) : 
        year_(year), month_(month), day_(day)
    {
        //
    }

    int year_ = 1970;
    int month_ = 1;
    int day_ = 1;

    time_t AsTimestamp() const {
        std::tm t;

        t.tm_sec   = 0;
        t.tm_min   = 0;
        t.tm_hour  = 0;
        t.tm_mday  = day_;
        t.tm_mon   = month_ - 1;
        t.tm_year  = year_ - 1900;
        t.tm_isdst = 0;
        return mktime(&t);
    }
};

int ComputeDaysDiff(const Date& date_from, const Date& date_to) {
    const time_t timestamp_to = date_to.AsTimestamp();
    const time_t timestamp_from = date_from.AsTimestamp();

    static const int SECONDS_IN_DAY = 60 * 60 * 24;
    return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}

int ComputeDay(const Date& date) {
    static const int SECONDS_IN_DAY = 60 * 60 * 24;
    const time_t timestamp = date.AsTimestamp();
    const time_t shift_timestamp = Date(1999, 1, 1).AsTimestamp();

    return (timestamp - shift_timestamp) / SECONDS_IN_DAY;
}

class BudgetManager {
    constexpr static const double TAX = 13;
    map<int, double> budget;
    map<int, double> spendings;

    void Earn(Date &from, Date &to, int value) {
        double day_value = 0;
        int days = 0;
        int day_from = ComputeDay(from);
        int day_to = ComputeDay(to);

        if (day_from > day_to) {
            throw invalid_argument("invalid date");
        }
        days = ComputeDaysDiff(from, to) + 1;
        day_value = (1.0 * value) / days;
        while (day_from <= day_to) {
            budget[day_from] += day_value;
            day_from++;
        }
    }

    void Spend(Date &from, Date &to, int value) {
        double day_value = 0;
        int days = 0;
        int day_from = ComputeDay(from);
        int day_to = ComputeDay(to);

        if (day_from > day_to) {
            throw invalid_argument("invalid date");
        }
        days = ComputeDaysDiff(from, to) + 1;
        day_value = (1.0 * value) / days;
        while (day_from <= day_to) {
            spendings[day_from] += day_value;
            day_from++;
        }
    }

    // void Pay(Date &from, Date &to) {
    //     int day_from = ComputeDay(from);
    //     int day_to = ComputeDay(to);

    //     if (day_from > day_to) {
    //         throw invalid_argument("invalid date");
    //     }
    //     while (day_from <= day_to) {
    //         budget[day_from] *= 1 - (TAX / 100);
    //         day_from++;
    //     }
    // }

    void Pay(Date &from, Date &to, double percentage = TAX) {
        int day_from = ComputeDay(from);
        int day_to = ComputeDay(to);

        if (day_from > day_to) {
            throw invalid_argument("invalid date");
        }
        while (day_from <= day_to) {
            budget[day_from] *= 1 - (percentage / 100);
            day_from++;
        }
    }

    void ComputeIncome(Date &from, Date &to, ostream &out) {
        int day_from = ComputeDay(from);
        int day_to = ComputeDay(to);
        double income = 0;

        if (day_from > day_to) {
            throw invalid_argument("invalid date");
        }
        while (day_from <= day_to) {
            income += budget[day_from];
            income -= spendings[day_from];
            day_from++;
        }
        out << setprecision(25) << income << "\n";
    }

public:
    BudgetManager() = default;

    void ProccessCommand(istream &input = cin, ostream &out = cout) {
        string command;
        Date from, to;
        optional<int> value;
        optional<double> percent;

        input >> command >> from >> to;
        if (command == "Earn") {
            input >> *value;
            Earn(from, to, *value);
        }
        else if (command == "ComputeIncome") {
            ComputeIncome(from, to, out);
        }
        else if (command == "PayTax") {
            input >> *percent;
            Pay(from, to, *percent);
            // Pay(from, to);

        }
        else if (command == "Spend") {
            input >> *value;
            Spend(from, to, *value);
        }
    }
};

int main() {
    BudgetManager manager;
    int query_count;

    ios::sync_with_stdio(false);
	cin.tie(nullptr);
    for (cin >> query_count; query_count > 0; --query_count) {
        manager.ProccessCommand(cin, cout);
    }
}