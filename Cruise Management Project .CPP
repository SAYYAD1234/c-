#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <ctime>

using namespace std;

// Forward declarations
class Cruise;
class Cabin;
class Passenger;

// Date structure for bookings
struct Date {
    int day, month, year;
    
    Date(int d = 1, int m = 1, int y = 2023) : day(d), month(m), year(y) {}
    
    string toString() const {
        return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    }
};

// Cabin class
class Cabin {
private:
    int number;
    string type;
    double price;
    bool available;
    Passenger* occupant;

public:
    Cabin(int num, string t, double p) 
        : number(num), type(t), price(p), available(true), occupant(nullptr) {}

    // Getters
    int getNumber() const { return number; }
    string getType() const { return type; }
    double getPrice() const { return price; }
    bool isAvailable() const { return available; }
    
    // Booking management
    bool bookCabin(Passenger* p);
    void freeCabin();
};

// Passenger class
class Passenger {
private:
    string name;
    string passportNumber;
    Date bookingDate;
    Cabin* assignedCabin;
    Cruise* bookedCruise;

public:
    Passenger(string n, string pn, Date bd) 
        : name(n), passportNumber(pn), bookingDate(bd), assignedCabin(nullptr), bookedCruise(nullptr) {}

    // Getters
    string getName() const { return name; }
    Cabin* getCabin() const { return assignedCabin; }
    Cruise* getCruise() const { return bookedCruise; }
    
    // Association methods
    void linkBooking(Cruise* c, Cabin* cab);
};

// Cruise class
class Cruise {
private:
    string cruiseId;
    string name;
    string route;
    Date departureDate;
    vector<Cabin> cabins;
    vector<Passenger*> passengers;

public:
    Cruise(string id, string n, string r, Date dd) 
        : cruiseId(id), name(n), route(r), departureDate(dd) {}

    // Cabin management
    void addCabin(int number, string type, double price) {
        cabins.emplace_back(number, type, price);
    }
    
    // Passenger management
    bool bookPassenger(Passenger* p, int cabinNumber);
    
    // Getters
    string getId() const { return cruiseId; }
    string getName() const { return name; }
    string getRoute() const { return route; }
    Date getDepartureDate() const { return departureDate; }
    
    // Display methods
    void showCabins() const;
    void showPassengers() const;
};

// Cruise Management System class
class CruiseManager {
private:
    vector<Cruise> cruises;
    vector<Passenger> passengers;
    
    // File persistence
    const string CRUISE_FILE = "cruises.dat";
    const string PASSENGER_FILE = "passengers.dat";

public:
    // Cruise operations
    void addCruise();
    void showAllCruises() const;
    Cruise* findCruise(const string& cruiseId);
    
    // Passenger operations
    void addPassenger();
    void showAllPassengers() const;
    
    // Booking system
    void makeBooking();
    
    // File operations
    void saveData();
    void loadData();
};

// Implementation of Cabin methods
bool Cabin::bookCabin(Passenger* p) {
    if (available) {
        available = false;
        occupant = p;
        return true;
    }
    return false;
}

void Cabin::freeCabin() {
    available = true;
    occupant = nullptr;
}

// Implementation of Passenger methods
void Passenger::linkBooking(Cruise* c, Cabin* cab) {
    bookedCruise = c;
    assignedCabin = cab;
}

// Implementation of Cruise methods
bool Cruise::bookPassenger(Passenger* p, int cabinNumber) {
    auto it = find_if(cabins.begin(), cabins.end(),
        [cabinNumber](const Cabin& c) { return c.getNumber() == cabinNumber && c.isAvailable(); });
    
    if (it != cabins.end()) {
        if (it->bookCabin(p)) {
            passengers.push_back(p);
            p->linkBooking(this, &(*it));
            return true;
        }
    }
    return false;
}

void Cruise::showCabins() const {
    cout << "\nAvailable Cabins for " << name << ":\n";
    cout << "-------------------------------------------------\n";
    cout << left << setw(10) << "Cabin No." << setw(15) << "Type" 
         << setw(10) << "Price" << "Status\n";
    
    for (const auto& cabin : cabins) {
        cout << left << setw(10) << cabin.getNumber()
             << setw(15) << cabin.getType()
             << setw(10) << fixed << setprecision(2) << cabin.getPrice()
             << (cabin.isAvailable() ? "Available" : "Booked") << endl;
    }
}

// Implementation of CruiseManager methods
void CruiseManager::addCruise() {
    string id, name, route;
    Date dd;
    
    cout << "\nEnter Cruise Details:\n";
    cout << "Cruise ID: ";
    cin >> id;
    cin.ignore();
    
    cout << "Cruise Name: ";
    getline(cin, name);
    
    cout << "Route: ";
    getline(cin, route);
    
    cout << "Departure Date (DD MM YYYY): ";
    cin >> dd.day >> dd.month >> dd.year;
    
    Cruise newCruise(id, name, route, dd);
    
    // Add cabins
    int cabinCount;
    cout << "Number of cabins: ";
    cin >> cabinCount;
    
    for (int i = 0; i < cabinCount; i++) {
        int num;
        string type;
        double price;
        
        cout << "Cabin " << (i+1) << ":\n";
        cout << "Number: "; cin >> num;
        cout << "Type: "; cin >> type;
        cout << "Price: "; cin >> price;
        
        newCruise.addCabin(num, type, price);
    }
    
    cruises.push_back(newCruise);
    cout << "Cruise added successfully!\n";
}

void CruiseManager::makeBooking() {
    string cruiseId;
    cout << "Enter Cruise ID: ";
    cin >> cruiseId;
    
    Cruise* cruise = findCruise(cruiseId);
    if (!cruise) {
        cout << "Cruise not found!\n";
        return;
    }
    
    cruise->showCabins();
    
    int cabinNumber;
    cout << "Enter Cabin Number: ";
    cin >> cabinNumber;
    
    // Create passenger
    string name, passport;
    Date bookingDate;
    
    cout << "Passenger Name: ";
    cin.ignore();
    getline(cin, name);
    
    cout << "Passport Number: ";
    getline(cin, passport);
    
    time_t now = time(0);
    tm* ltm = localtime(&now);
    bookingDate = Date(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    
    passengers.emplace_back(name, passport, bookingDate);
    Passenger* newPassenger = &passengers.back();
    
    if (cruise->bookPassenger(newPassenger, cabinNumber)) {
        cout << "Booking successful!\n";
    } else {
        cout << "Booking failed! Cabin unavailable.\n";
        passengers.pop_back();
    }
}

// File operations implementation
void CruiseManager::saveData() {
    // Implementation for saving to files
    // (Would serialize objects to binary/text files)
}

void CruiseManager::loadData() {
    // Implementation for loading from files
    // (Would deserialize objects from binary/text files)
}

// Main menu and program execution
int main() {
    CruiseManager manager;
    manager.loadData();
    
    while (true) {
        cout << "\nCruise Management System\n";
        cout << "1. Add New Cruise\n";
        cout << "2. Show All Cruises\n";
        cout << "3. Make Booking\n";
        cout << "4. Show All Passengers\n";
        cout << "5. Save Data\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                manager.addCruise();
                break;
            case 2:
                manager.showAllCruises();
                break;
            case 3:
                manager.makeBooking();
                break;
            case 4:
                manager.showAllPassengers();
                break;
            case 5:
                manager.saveData();
                break;
            case 6:
                manager.saveData();
                exit(0);
            default:
                cout << "Invalid choice!\n";
        }
    }
    return 0;
}