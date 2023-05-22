#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

class Date {
public:
    Date(int y, int m, int d) {
        year = y;
        if (m > 12 || m < 1) throw logic_error("Month value is invalid: " + to_string(m));
        month = m;
        if (d > 31 || d < 1) throw logic_error("Day value is invalid: " + to_string(d));
        day = d;
    }
    int GetYear() const {return year;}
    int GetMonth() const {return month;}
    int GetDay() const {return day;}
private:
    int year, month, day;
};

bool operator<(const Date& lhs, const Date& rhs) {
    return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
        vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

ostream& operator<<(ostream& stream, const Date& date) {
    stream << setw(4) << setfill('0') << date.GetYear() <<
        "-" << setw(2) << setfill('0') << date.GetMonth() <<
        "-" << setw(2) << setfill('0') << date.GetDay();
    return stream;
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        events[date].insert(event);
        }

    bool DeleteEvent(const Date& date, const string& event) {
        if (events.count(date) > 0 && events[date].count(event) > 0) {
            events[date].erase(event);
            return true;
        }
        return false;
    }

    int DeleteDate(const Date& date) {
        if (events.count(date) == 0) return 0;

        else {
            const int event_count = events[date].size();
            events.erase(date);
            return event_count;
        }
    }

    set<string> Find(const Date& date) const {
        if (events.count(date) > 0) return events.at(date);

        else return {};
    }

    void Print() const {

        for (const auto& i : events) {
            for (const string& event : i.second) cout << i.first << " " << event << endl;
        }
    }

private:
    map<Date, set<string>> events;
};

Date ParseDate(const string& date) {
    istringstream date_stream(date);
    bool check = true;
    int year;
    check = check && (date_stream >> year);
    check = check && (date_stream.peek() == '-');
    date_stream.ignore(1);
    int month;
    check = check && (date_stream >> month);
    check = check && (date_stream.peek() == '-');
    date_stream.ignore(1);
    int day;
    check = check && (date_stream >> day);
    check = check && date_stream.eof();
    if (check == false) throw logic_error("Wrong date format: " + date);
    return Date(year, month, day);
}

int main() {
    try {
        Database database;

        string cmd;

        while (getline(cin, cmd)) {
            stringstream ss(cmd);
            string command;
            ss >> command;
            if (command == "Add") {
                string date_str, event;
                ss >> date_str >> event;
                const Date date = ParseDate(date_str);
                database.AddEvent(date, event);
            }
            else if (command == "Del") {
                string date_str;
                ss >> date_str;
                string event;
                if (!ss.eof()) ss >> event;
                const Date date = ParseDate(date_str);
                if (event.empty()) {
                    const int count = database.DeleteDate(date);
                    cout << "Deleted " << count << " events" << endl;
                }
                else {
                    if (database.DeleteEvent(date, event)) cout << "Deleted successfully" << endl;

                    else cout << "Event not found" << endl;
                }
            }

            else if (command == "Find") {
                string date_str;
                ss >> date_str;
                const Date date = ParseDate(date_str);
                for (const string& event : database.Find(date)) cout << event << endl;
            }

            else if (command == "Print") database.Print();

            else if (!command.empty()) throw logic_error("Unknown command: " + command);
        }
    }

    catch (const exception& e) {
        cout << e.what() << endl;
    }
    return 0;
}