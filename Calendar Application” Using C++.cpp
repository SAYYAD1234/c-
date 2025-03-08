#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;

// Function prototypes
bool isLeapYear(int year);
int getDaysInMonth(int month, int year);
int getDayOfWeek(int day, int month, int year);
string formatDate(int year, int month, int day);
string getMonthName(int month);
void printCalendar(int month, int year, const map<string, vector<string>>& events);
void addEvent(map<string, vector<string>>& events);
void saveEvents(const map<string, vector<string>>& events, const string& filename);
void loadEvents(map<string, vector<string>>& events, const string& filename);

int main() {
    int currentMonth = 10; // Default to October
    int currentYear = 2023;
    map<string, vector<string>> events;
    const string filename = "calendar_events.txt";

    loadEvents(events, filename);

    while (true) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        
        printCalendar(currentMonth, currentYear, events);
        
        cout << "\nCalendar Options:\n";
        cout << "N - Next month\tP - Previous month\n";
        cout << "Y - Change year\tA - Add event\n";
        cout << "V - View events\tS - Save events\n";
        cout << "L - Load events\tQ - Quit\n";
        cout << "Enter choice: ";

        char choice;
        cin >> choice;
        choice = toupper(choice);

        switch (choice) {
            case 'N':
                currentMonth = (currentMonth % 12) + 1;
                if (currentMonth == 1) currentYear++;
                break;
            case 'P':
                currentMonth = (currentMonth == 1) ? 12 : currentMonth - 1;
                if (currentMonth == 12) currentYear--;
                break;
            case 'Y':
                cout << "Enter new year: ";
                cin >> currentYear;
                break;
            case 'A':
                addEvent(events);
                break;
            case 'V': {
                int year, month, day;
                cout << "Enter date (YYYY MM DD): ";
                cin >> year >> month >> day;
                string dateKey = formatDate(year, month, day);
                if (events.find(dateKey) != events.end()) {
                    cout << "\nEvents on " << dateKey << ":\n";
                    for (const auto& event : events.at(dateKey)) {
                        cout << " • " << event << "\n";
                    }
                } else {
                    cout << "\nNo events for this date.\n";
                }
                cin.ignore();
                cin.get();
                break;
            }
            case 'S':
                saveEvents(events, filename);
                break;
            case 'L':
                loadEvents(events, filename);
                break;
            case 'Q':
                saveEvents(events, filename);
                exit(0);
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int getDaysInMonth(int month, int year) {
    if (month == 2) return isLeapYear(year) ? 29 : 28;
    if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    return 31;
}

int getDayOfWeek(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year--;
    }
    int q = day;
    int m = month;
    int K = year % 100;
    int J = year / 100;
    int h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
    return (h + 6) % 7;
}

string formatDate(int year, int month, int day) {
    ostringstream oss;
    oss << year << "-" 
        << setw(2) << setfill('0') << month << "-"
        << setw(2) << setfill('0') << day;
    return oss.str();
}

string getMonthName(int month) {
    const string months[] = {"January", "February", "March", "April", "May", "June",
                           "July", "August", "September", "October", "November", "December"};
    return months[month - 1];
}

void printCalendar(int month, int year, const map<string, vector<string>>& events) {
    cout << "\n " << getMonthName(month) << " " << year << "\n";
    cout << " Sun Mon Tue Wed Thu Fri Sat\n";

    int daysInMonth = getDaysInMonth(month, year);
    int startDay = getDayOfWeek(1, month, year);
    int currentDayOfWeek = startDay;

    for (int i = 0; i < startDay; i++) {
        cout << "    ";
    }

    for (int day = 1; day <= daysInMonth; day++) {
        string dateKey = formatDate(year, month, day);
        bool hasEvents = events.find(dateKey) != events.end();
        cout << setw(3) << day << (hasEvents ? "* " : "  ");
        currentDayOfWeek++;
        if (currentDayOfWeek == 7) {
            cout << endl;
            currentDayOfWeek = 0;
        }
    }
    if (currentDayOfWeek != 0) {
        cout << endl;
    }
}

void addEvent(map<string, vector<string>>& events) {
    int year, month, day;
    cout << "\nEnter date (YYYY MM DD): ";
    cin >> year >> month >> day;
    string dateKey = formatDate(year, month, day);
    cin.ignore();
    string description;
    cout << "Enter event description: ";
    getline(cin, description);
    events[dateKey].push_back(description);
    cout << "Event added successfully!\n";
}

void saveEvents(const map<string, vector<string>>& events, const string& filename) {
    ofstream file(filename);
    for (const auto& entry : events) {
        for (const auto& event : entry.second) {
            file << entry.first << ":" << event << "\n";
        }
    }
    cout << "\nEvents saved to " << filename << "\n";
}

void loadEvents(map<string, vector<string>>& events, const string& filename) {
    ifstream file(filename);
    events.clear();
    string line;
    while (getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos != string::npos) {
            string date = line.substr(0, colonPos);
            string event = line.substr(colonPos + 1);
            events[date].push_back(event);
        }
    }
    cout << "\nEvents loaded from " << filename << "\n";
}
