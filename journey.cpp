#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <string>
#include <algorithm>
#include <cstdlib>
using namespace std;

struct TransportOption {
    string name;
    double cost;
    double time;
    int comfort;

    TransportOption(string name, double cost, double time, int comfort)
        : name(move(name)), cost(cost), time(time), comfort(comfort) {}
};

struct Trip {
    string start, end, status, id;
    double cost;

    Trip(string start, string end, string status, double cost, string id)
        : start(move(start)), end(move(end)), status(move(status)), cost(cost), id(move(id)) {}
};

class NotificationQueue {
    queue<string> notifications;

public:
    void add(const string& message) { notifications.push(message); }
    void process() {
        while (!notifications.empty()) {
            cout << "Notification: " << notifications.front() << "\n";
            notifications.pop();
        }
    }
};

class User {
    string username;
    double wallet = 100.0;
    list<Trip> trips;

public:
    explicit User(string name) : username(move(name)) {}

    const string& getName() const { return username; }

    void addTrip(const Trip& trip) { trips.push_back(trip); }

    void viewTrips() const {
        cout << "Trip History for " << username << ":\n";
        for (const auto& trip : trips) {
            cout << "ID: " << trip.id << ", From: " << trip.start << " to " << trip.end
                 << ", Status: " << trip.status << ", Cost: $" << trip.cost << "\n";
        }
    }

    bool pay(double amount) {
        if (wallet >= amount) {
            wallet -= amount;
            cout << "Payment of $" << amount << " successful. Remaining balance: $" << wallet << "\n";
            return true;
        }
        cout << "Insufficient funds! Balance: $" << wallet << "\n";
        return false;
    }
};

class Station {
public:
    string name;
    vector<Station*> connections;
    vector<TransportOption> options;

    explicit Station(string name) : name(move(name)) {}

    void addConnection(Station* station) { connections.push_back(station); }

    void addOption(const TransportOption& option) { options.push_back(option); }

    void displayOptions() const {
        cout << "Transport options from " << name << ":\n";
        for (const auto& opt : options) {
            cout << "- " << opt.name << " | Cost: $" << opt.cost << ", Time: " << opt.time
                 << " hrs, Comfort: " << opt.comfort << " stars\n";
        }
    }
};

class Segment {
    Station* start;
    Station* end;
    vector<TransportOption> options;

public:
    Segment(Station* start, Station* end, vector<TransportOption> options)
        : start(start), end(end), options(move(options)) {}

    TransportOption chooseOption(int comfort, bool prioritizeTime) const {
        vector<TransportOption> filtered;
        copy_if(options.begin(), options.end(), back_inserter(filtered),
                [&](const auto& opt) { return opt.comfort >= comfort; });

        sort(filtered.begin(), filtered.end(), [&](const auto& a, const auto& b) {
            return prioritizeTime ? a.time < b.time : a.cost < b.cost;
        });

        cout << "Available options:\n";
        for (size_t i = 0; i < filtered.size(); ++i) {
            cout << i + 1 << ". " << filtered[i].name << " | Cost: $" << filtered[i].cost
                 << ", Time: " << filtered[i].time << " hrs, Comfort: " << filtered[i].comfort << " stars\n";
        }

        int choice;
        cout << "Choose an option (1-" << filtered.size() << "): ";
        cin >> choice;

        return filtered[choice - 1];
    }
};

class Journey {
    vector<Segment> segments;

public:
    void addSegment(const Segment& seg) { segments.push_back(seg); }

    void summarize(int comfort, bool prioritizeTime) const {
        double totalCost = 0, totalTime = 0;

        for (const auto& seg : segments) {
            auto option = seg.chooseOption(comfort, prioritizeTime);
            totalCost += option.cost;
            totalTime += option.time;
        }

        cout << "\nJourney Summary:\n";
        cout << "Total Time: " << totalTime << " hrs\n";
        cout << "Total Cost: $" << totalCost << "\n";
    }
};

// Helper functions and global objects
vector<User> users;
NotificationQueue notifications;

User* findUser(const string& name) {
    for (auto& user : users)
        if (user.getName() == name)
            return &user;
    return nullptr;
}

void mainMenu() {
    cout << "\n1. Sign Up\n2. Log In\n3. Exit\nChoose: ";
}

void userMenu() {
    cout << "\n1. Plan Journey\n2. View Trip History\n3. Make Payment\n4. Log Out\nChoose: ";
}

int main() {
    while (true) {
        mainMenu();
        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "Enter username: ";
            string name;
            cin >> name;
            if (!findUser(name)) {
                users.emplace_back(name);
                cout << "Sign-up successful!\n";
            } else {
                cout << "Username already exists.\n";
            }
        } else if (choice == 2) {
            cout << "Enter username: ";
            string name;
            cin >> name;
            if (auto* user = findUser(name)) {
                while (true) {
                    userMenu();
                    cin >> choice;

                    if (choice == 1) {
                        Station A("Station A"), B("Station B");
                        A.addOption({"Bus", 50, 5, 3});
                        A.addOption({"Train", 80, 3, 4});
                        A.addOption({"Flight", 200, 1, 5});

                        Journey journey;
                        journey.addSegment({&A, &B, A.options});
                        journey.summarize(3, true); // Example: comfort 3, prioritize time
                        notifications.add("Journey planned!");
                    } else if (choice == 2) {
                        user->viewTrips();
                    } else if (choice == 3) {
                        cout << "Enter amount: ";
                        double amt;
                        cin >> amt;
                        user->pay(amt);
                    } else if (choice == 4) {
                        break;
                    } else {
                        cout << "Invalid choice.\n";
                    }
                }
            } else {
                cout << "User not found.\n";
            }
        } else if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}
















