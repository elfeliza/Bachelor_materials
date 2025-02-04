#include <string>
#include <set>
#include <iostream>
#include "profile.h"
#include <deque>
#include <map>
#include <utility>

using namespace std;

struct Reserve
{
  explicit Reserve(int64_t T, int cl_id, int r_c)
  : time(T)
  , client_id(cl_id)
  , room_count(r_c)
  {}
  int64_t time;
  int client_id;
  int room_count;
};

class HotelSystem
{
public:

  HotelSystem(): current_time(0) {}

  void BOOK (int64_t time, string hotel_name, int client_id,
             int room_count)
  {
    current_time = time;
    Reserve reserve(time, client_id, room_count);
    orders[hotel_name].push_back(reserve);
    clients[hotel_name][client_id] += room_count;
    rooms[hotel_name] += room_count;
  }

  int CLIENTS (string hotel_name)
  {
    Adjust(hotel_name);
    return clients[hotel_name].size();
  }
  int ROOMS (string hotel_name)
  {
    Adjust(hotel_name);
    if (rooms[hotel_name] <= 0) return 0;
    return rooms[hotel_name];
  }

private:
int64_t current_time;
map <string, deque<Reserve>> orders;
map <string, map<int, int>> clients; // <hotel_name, <client_id, room_count>>
map <string, int> rooms;

void Adjust(string hotel_name)
{
  if (orders.count(hotel_name) == 0) return;

  deque<Reserve>& tmp = orders[hotel_name];

  while (!tmp.empty() && tmp.front().time <= current_time - 86400)
  {
    rooms[hotel_name] -= tmp.front().room_count;
    if (clients[hotel_name].count(tmp.front().client_id))
    {
      clients[hotel_name][tmp.front().client_id] -= tmp.front().room_count;
      if (clients[hotel_name][tmp.front().client_id] <= 0)
      {
        clients[hotel_name].erase(tmp.front().client_id);
      }
    }
    tmp.pop_front();
  }

}
};

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  HotelSystem system;
  int Q = 0;
  cin >> Q;

  for (int i  = 0; i < Q; ++i)
  {
    string command;
    cin >> command;

    if (command == "BOOK")
    {
      string hotel_name;
      int64_t time = 0;
      int client_id = 0;
      int room_count = 0;
      cin >> time;
      cin >> hotel_name;
      cin >> client_id >> room_count;
      system.BOOK(time, hotel_name, client_id, room_count);
    }

    else if (command == "CLIENTS")
    {
      string hotel_name;
      cin >> hotel_name;
      cout << system.CLIENTS(hotel_name) << '\n';
    }

    else if (command == "ROOMS")
    {
      string hotel_name;
      cin >> hotel_name;
      cout << system.ROOMS(hotel_name) << '\n';
    }
  }
  return 0;
}
/* Авторское решение:

#include <cstdint>
#include <iostream>
#include <map>
#include <queue>

using namespace std;


class HotelManager {
public:
  void Book(int64_t time, const string& hotel_name,
            int client_id, int room_count) {
    current_time_ = time;
    hotels_[hotel_name].Book({time, client_id, room_count});
  }
  int ComputeClientCount(const string& hotel_name) {
    return hotels_[hotel_name].ComputeClientCount(current_time_);
  }
  int ComputeRoomCount(const string& hotel_name) {
    return hotels_[hotel_name].ComputeRoomCount(current_time_);
  }

private:
  struct Booking {
    int64_t time;
    int client_id;
    int room_count;
  };

  class HotelInfo {
  public:
    void Book(const Booking& booking) {
      last_bookings_.push(booking);
      room_count_ += booking.room_count;
      ++client_booking_counts_[booking.client_id];
    }
    int ComputeClientCount(int64_t current_time) {
      RemoveOldBookings(current_time);
      return client_booking_counts_.size();
    }
    int ComputeRoomCount(int64_t current_time) {
      RemoveOldBookings(current_time);
      return room_count_;
    }
  private:
    static const int TIME_WINDOW_SIZE = 86400;
    queue<Booking> last_bookings_;
    int room_count_ = 0;
    map<int, int> client_booking_counts_;

    void PopBooking() {
      const Booking& booking = last_bookings_.front();
      room_count_ -= booking.room_count;
      const auto client_stat_it =
          client_booking_counts_.find(booking.client_id);
      if (--client_stat_it->second == 0) {
        client_booking_counts_.erase(client_stat_it);
      }
      last_bookings_.pop();
    }
    void RemoveOldBookings(int64_t current_time) {
      while (
          !last_bookings_.empty()
          && last_bookings_.front().time <= current_time - TIME_WINDOW_SIZE
      ) {
        PopBooking();
      }
    }
  };

  int64_t current_time_ = 0;
  map<string, HotelInfo> hotels_;
};


int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  HotelManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;

    if (query_type == "BOOK") {
      int64_t time;
      cin >> time;
      string hotel_name;
      cin >> hotel_name;
      int client_id, room_count;
      cin >> client_id >> room_count;
      manager.Book(time, hotel_name, client_id, room_count);
    } else {
      string hotel_name;
      cin >> hotel_name;
      if (query_type == "CLIENTS") {
        cout << manager.ComputeClientCount(hotel_name) << "\n";
      } else if (query_type == "ROOMS") {
        cout << manager.ComputeRoomCount(hotel_name) << "\n";
      }
    }
  }

  return 0;
}




*/
