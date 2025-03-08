#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <ctime>

using namespace std;

// Forward declarations
class Train;
class Passenger;
class Booking;

// Train Class
class Train {
private:
    int trainNumber;
    string trainName;
    string source;
    string destination;
    int acSeats;
    int sleeperSeats;
    int acFare;
    int sleeperFare;

public:
    Train(int num, string name, string src, string dest, int ac, int slp, int acF, int slpF)
    : trainNumber(num), trainName(name), source(src), destination(dest),
      acSeats(ac), sleeperSeats(slp), acFare(acF), sleeperFare(slpF) {}

    // Getters
    int getNumber() const { return trainNumber; }
    string getName() const { return trainName; }
    string getRoute() const { return source + " to " + destination; }
    int getACSeats() const { return acSeats; }
    int getSleeperSeats() const { return sleeperSeats; }
    int getACFare() const { return acFare; }
    int getSleeperFare() const { return sleeperFare; }

    // Seat management
    bool bookSeats(string cls, int count) {
        if (cls == "AC") {
            if (acSeats >= count) {
                acSeats -= count;
                return true;
            }
        } else if (cls == "Sleeper") {
            if (sleeperSeats >= count) {
                sleeperSeats -= count;
                return true;
            }
        }
        return false;
    }

    void cancelSeats(string cls, int count) {
        if (cls == "AC") acSeats += count;
        else if (cls == "Sleeper") sleeperSeats += count;
    }
};

// Passenger Class
class Passenger {
private:
    string name;
    int age;
    string gender;

public:
    Passenger(string n, int a, string g) : name(n), age(a), gender(g) {}

    string getDetails() const {
        return "Name: " + name + "\nAge: " + to_string(age) + "\nGender: " + gender;
    }
};

// Booking Class
class Booking {
private:
    static int nextPNR;
    int pnr;
    Train* train;
    Passenger* passenger;
    string travelClass;
    int numOfSeats;
    string journeyDate;

public:
    Booking(Train* t, Passenger* p, string cls, int seats, string date)
    : train(t), passenger(p), travelClass(cls), numOfSeats(seats), journeyDate(date) {
        pnr = nextPNR++;
    }

    int getPNR() const { return pnr; }
    string getDetails() const {
        return "PNR: " + to_string(pnr) + "\nTrain: " + train->getName() +
               "\nClass: " + travelClass + "\nSeats: " + to_string(numOfSeats) +
               "\nDate: " + journeyDate + "\n" + passenger->getDetails();
    }

    Train* getTrain() const { return train; }
    string getClass() const { return travelClass; }
    int getSeats() const { return numOfSeats; }
};

int Booking::nextPNR = 1000;

// Railway System Class
class RailwaySystem {
private:
    vector<Train> trains;
    vector<Passenger> passengers;
    vector<Booking> bookings;
    
    // File names
    const string TRAIN_FILE = "trains.dat";
    const string BOOKING_FILE = "bookings.dat";

    // File operations
    void saveTrains() {
        ofstream file(TRAIN_FILE);
        for (auto& t : trains) {
            file << t.getNumber() << "," << t.getName() << "," 
                 << t.getRoute() << "," << t.getACSeats() << ","
                 << t.getSleeperSeats() << "\n";
        }
    }

    void loadTrains() {
        ifstream file(TRAIN_FILE);
        // Implementation similar to save but parsing data
    }

    void saveBookings() {
        ofstream file(BOOKING_FILE);
        for (auto& b : bookings) {
            file << b.getPNR() << "," << b.getTrain()->getNumber() << ","
                 << b.getClass() << "," << b.getSeats() << "\n";
        }
    }

public:
    RailwaySystem() {
        loadTrains();
        // Load bookings from file
    }

    ~RailwaySystem() {
        saveTrains();
        saveBookings();
    }

    void addTrain() {
        int num, ac, slp, acF, slpF;
        string name, src, dest;

        cout << "\nEnter Train Details:\n";
        cout << "Train Number: "; cin >> num;
        cout << "Train Name: "; cin.ignore(); getline(cin, name);
        cout << "Source: "; getline(cin, src);
        cout << "Destination: "; getline(cin, dest);
        cout << "AC Seats: "; cin >> ac;
        cout << "Sleeper Seats: "; cin >> slp;
        cout << "AC Fare: "; cin >> acF;
        cout << "Sleeper Fare: "; cin >> slpF;

        trains.emplace_back(num, name, src, dest, ac, slp, acF, slpF);
        cout << "Train added successfully!\n";
    }

    void showTrains() {
        cout << "\nAvailable Trains:\n";
        cout << "------------------------------------------------------------\n";
        cout << left << setw(10) << "Number" << setw(20) << "Name" 
             << setw(25) << "Route" << setw(10) << "AC Seats" 
             << setw(12) << "Sleeper Seats" << endl;

        for (auto& t : trains) {
            cout << left << setw(10) << t.getNumber() 
                 << setw(20) << t.getName()
                 << setw(25) << t.getRoute()
                 << setw(10) << t.getACSeats()
                 << setw(12) << t.getSleeperSeats() << endl;
        }
    }

    void bookTicket() {
        int trainNum, seats, age;
        string name, gender, cls, date;
        
        showTrains();
        cout << "\nEnter Train Number: "; cin >> trainNum;
        
        auto it = find_if(trains.begin(), trains.end(), 
            [trainNum](const Train& t) { return t.getNumber() == trainNum; });
        
        if (it == trains.end()) {
            cout << "Train not found!\n";
            return;
        }

        cout << "Enter Class (AC/Sleeper): "; cin >> cls;
        cout << "Number of Seats: "; cin >> seats;
        cout << "Journey Date (DD-MM-YYYY): "; cin >> date;
        
        cout << "\nPassenger Details:\n";
        cout << "Name: "; cin.ignore(); getline(cin, name);
        cout << "Age: "; cin >> age;
        cout << "Gender: "; cin >> gender;

        passengers.emplace_back(name, age, gender);
        
        if (it->bookSeats(cls, seats)) {
            bookings.emplace_back(&(*it), &passengers.back(), cls, seats, date);
            cout << "Booking successful! PNR: " 
                 << bookings.back().getPNR() << endl;
        } else {
            cout << "Booking failed! Not enough seats.\n";
            passengers.pop_back();
        }
    }

    void cancelTicket() {
        int pnr;
        cout << "Enter PNR Number: "; cin >> pnr;
        
        auto it = find_if(bookings.begin(), bookings.end(),
            [pnr](const Booking& b) { return b.getPNR() == pnr; });
        
        if (it != bookings.end()) {
            it->getTrain()->cancelSeats(it->getClass(), it->getSeats());
            bookings.erase(it);
            cout << "Cancellation successful!\n";
        } else {
            cout << "Invalid PNR number!\n";
        }
    }
};

// Main menu and program execution
int main() {
    RailwaySystem system;
    
    while (true) {
        cout << "\nRailway Reservation System\n";
        cout << "1. Add Train\n";
        cout << "2. Show Trains\n";
        cout << "3. Book Ticket\n";
        cout << "4. Cancel Ticket\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                system.addTrain();
                break;
            case 2:
                system.showTrains();
                break;
            case 3:
                system.bookTicket();
                break;
            case 4:
                system.cancelTicket();
                break;
            case 5:
                exit(0);
            default:
                cout << "Invalid choice!\n";
        }
    }
    return 0;
}