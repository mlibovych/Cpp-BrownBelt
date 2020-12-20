#include "test_runner.h"

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct Record {
    string id;
    string title;
    string user;
    int timestamp;
    int karma;

    bool operator<(const Record& other) const {
        return std::tie(id, title, user, timestamp, karma) <
               std::tie(other.id, other.title, other.user, other.timestamp, other.karma);
    }

    bool operator==(const Record& other) const {
        return std::tie(id, title, user, timestamp, karma) ==
               std::tie(other.id, other.title, other.user, other.timestamp, other.karma);
    }
};

struct Indexes {
    std::multimap<int, std::set<Record>::iterator>::iterator tidx;
    std::multimap<int, std::set<Record>::iterator>::iterator kidx;
    std::multimap<std::string, std::set<Record>::iterator>::iterator uidx;
};

class Database {
    std::set<Record> data;
    
    std::unordered_map<std::string, std::set<Record>::iterator> id_idx;
    std::multimap<int, std::set<Record>::iterator> timestamp_idx;
    std::multimap<int, std::set<Record>::iterator> karma_idx;
    std::multimap<std::string, std::set<Record>::iterator> user_idx;
    std::unordered_map<std::string, Indexes> indexes;
public:
    bool Put(const Record& record);
    const Record* GetById(const string& id) const;
    bool Erase(const string& id);

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const;

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const;

    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const;
};

bool Database::Put(const Record& record) {
    if (!id_idx.count(record.id)) {
        auto tmp = data.insert(record);

        if (tmp.second) {
            id_idx[record.id] = tmp.first;
            auto tidx = timestamp_idx.insert(decltype(timestamp_idx)::value_type(record.timestamp, tmp.first));
            auto kidx = karma_idx.insert(decltype(karma_idx)::value_type(record.karma, tmp.first));
            auto uidx = user_idx.insert(decltype(user_idx)::value_type(record.user, tmp.first));
            indexes[record.id] = {tidx, kidx, uidx};
            return 1;
        }
    }
    return 0;
}

const Record* Database::GetById(const string& id) const {
    if (!id_idx.count(id)) {
        return nullptr;
    }
    return &*id_idx.at(id);
}

bool Database::Erase(const string& id) {
    if (id_idx.count(id)) {
        timestamp_idx.erase(indexes[id].tidx);   
        karma_idx.erase(indexes[id].kidx);
        user_idx.erase(indexes[id].uidx);
        indexes.erase(id);

        data.erase(id_idx.at(id));
        id_idx.erase(id);
        return 1;
    }
    return 0;
}

template <typename Callback>
void Database::RangeByTimestamp(int low, int high, Callback callback) const {
    auto first = timestamp_idx.lower_bound(low);
    auto second = timestamp_idx.upper_bound(high);

    while (first != second) {
        if (!callback(*(*first).second)) {
            break;
        }
        first++;
    }
}

template <typename Callback>
void Database::RangeByKarma(int low, int high, Callback callback) const {
    auto first = karma_idx.lower_bound(low);
    auto second = karma_idx.upper_bound(high);

    while (first != second) {
        if (!callback(*(*first).second)) {
            break;
        }
        first++;
    }
}

template <typename Callback>
void Database::AllByUser(const string& user, Callback callback) const {
    auto first = user_idx.lower_bound(user);
    auto second = user_idx.upper_bound(user);

    while (first != second) {
        if (!callback(*(*first).second)) {
            break;
        }
        first++;
    }
}


