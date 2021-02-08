#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <set>
#include <numeric>
#include <deque>

using namespace std;

class Booking {
    deque <tuple<int64_t, string, int64_t, int64_t>> events;
    map<string, int64_t> rooms;
    map<string, map<int64_t, int64_t>> clients;
public :
    Booking() = default;

    void Book() {
        int64_t time, client_id, room_count;
        string hotel_name;

        cin >> time >> hotel_name >> client_id >> room_count;
        events.push_back(make_tuple(time, hotel_name, client_id, room_count));
        rooms[hotel_name] += room_count;
        clients[hotel_name][client_id] += 1;

        auto it = events.begin();

        while (get<0>(*it) <= time - 86400) {
            rooms[get<1>(*it)] -= get<3>(*it);
            clients[get<1>(*it)][get<2>(*it)] -= 1;
            if (clients[get<1>(*it)][get<2>(*it)] == 0) {
                clients[get<1>(*it)].erase(get<2>(*it));
            }
            it++;
            events.pop_front();
        }
    }

    const int GetRooms(const string& hotel_name) const {
        if (!rooms.count(hotel_name)) {
            return 0;
        }
        return rooms.at(hotel_name);
    }

    const int GetClients(const string& hotel_name) const {
        if (!clients.count(hotel_name)) {
            return 0;
        }
        return clients.at(hotel_name).size();
    }
};

int main() {
    Booking booking;
    int Q;

    ios::sync_with_stdio(false);
	cin.tie(nullptr);
    for (cin >> Q; Q > 0; --Q) {
        string command;

        cin >> command;
        if (command == "BOOK") {
            booking.Book();
        }
        else if (command == "ROOMS") {
            string hotel_name;

            cin >> hotel_name;
            cout << booking.GetRooms(hotel_name) << endl;
        }
        else if (command == "CLIENTS") {
            string hotel_name;

            cin >> hotel_name;
            cout << booking.GetClients(hotel_name) << endl;
        }
    }
}