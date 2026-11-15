
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <fstream>
#include <map>
#include <cctype>
#include <cmath>

using namespace std;

const double DEFAULT_CONSUMPTION = 0.18;
const double MAX_DETOUR_KM = 3.0;
const double INF = numeric_limits<double>::infinity();

// ============================================================
// 1. USER REGISTRATION AND LOGIN SYSTEM
// ============================================================

class UserAuth
{
private:
    struct Account
    {
        string username;
        string password;
    };

    map<string, Account> accounts;
    string fileName;

    string normalize(const string& name) const
    {
        string result;

        for (char ch : name)
            result += static_cast<char>(
                tolower(static_cast<unsigned char>(ch)));

        return result;
    }

    bool validInput(const string& input) const
    {
        if (input.empty()) return false;

        for (char ch : input)
        {
            if (isspace(static_cast<unsigned char>(ch)))
                return false;
        }

        return true;
    }

    void loadAccounts()
    {
        ifstream fin(fileName.c_str());

        string username, password;

        while (fin >> username >> password)
        {
            Account account;
            account.username = username;
            account.password = password;

            accounts[normalize(username)] = account;
        }
    }

    void saveAccounts() const
    {
        ofstream fout(fileName.c_str());

        if (!fout)
            throw runtime_error("Unable to save account information.");

        for (const auto& item : accounts)
        {
            fout << item.second.username << " "
                 << item.second.password << '\n';
        }
    }

    bool registerUser(string& loggedInUser)
    {
        string username, password, confirmPassword;

        cout << "\n========== NEW USER REGISTRATION ==========\n";

        cout << "Create username: ";
        getline(cin, username);

        if (!validInput(username))
        {
            cout << "Invalid username. Spaces and empty usernames are not allowed.\n";
            return false;
        }

        string key = normalize(username);

        if (accounts.find(key) != accounts.end())
        {
            cout << "Username already exists! Please choose another username.\n";
            return false;
        }

        cout << "Create password: ";
        getline(cin, password);

        if (!validInput(password))
        {
            cout << "Password cannot be empty or contain spaces.\n";
            return false;
        }

        if (password.length() < 4)
        {
            cout << "Password must contain at least 4 characters.\n";
            return false;
        }

        cout << "Confirm password: ";
        getline(cin, confirmPassword);

        if (password != confirmPassword)
        {
            cout << "Passwords do not match. Registration failed.\n";
            return false;
        }

        Account newAccount;
        newAccount.username = username;
        newAccount.password = password;

        accounts[key] = newAccount;

        saveAccounts();

        cout << "\nRegistration successful!\n";
        cout << "Welcome, " << username << "!\n";

        loggedInUser = username;

        return true;
    }

    bool loginUser(string& loggedInUser)
    {
        string username, password;

        cout << "\n================ USER LOGIN ================\n";

        cout << "Enter username: ";
        getline(cin, username);

        cout << "Enter password: ";
        getline(cin, password);

        string key = normalize(username);

        auto it = accounts.find(key);

        if (it == accounts.end())
        {
            cout << "\nAccount not found. Please register first.\n";
            return false;
        }

        if (it->second.password != password)
        {
            cout << "\nIncorrect password! Login failed.\n";
            return false;
        }

        loggedInUser = it->second.username;

        cout << "\nLogin successful!\n";
        cout << "Welcome back, " << loggedInUser << "!\n";

        return true;
    }

public:
    UserAuth(const string& file = "users.txt")
        : fileName(file)
    {
        loadAccounts();
    }

    bool authenticate(string& loggedInUser)
    {
        while (true)
        {
            string choice;

            cout << "\n====================================================\n";
            cout << "          SMART EV CHARGING SPOT LOCATOR            \n";
            cout << "====================================================\n";

            cout << "\n1. Register New Account\n";
            cout << "2. Login to Existing Account\n";
            cout << "3. Exit\n";

            cout << "\nEnter your choice: ";
            getline(cin, choice);

            if (choice == "1")
            {
                if (registerUser(loggedInUser))
                    return true;
            }
            else if (choice == "2")
            {
                if (loginUser(loggedInUser))
                    return true;
            }
            else if (choice == "3")
            {
                cout << "\nThank you for using Smart EV Charging Spot Locator!\n";
                return false;
            }
            else
            {
                cout << "Invalid choice. Please select 1, 2 or 3.\n";
            }
        }
    }
};

// ============================================================
// 2. AUTOMATIC CAR ID GENERATOR
// ============================================================

class CarIDGenerator
{
private:
    map<string, int> lastNumber;
    string fileName;

    string makePrefix(const string& name) const
    {
        string prefix;

        for (char ch : name)
        {
            if (isalnum(static_cast<unsigned char>(ch)))
                prefix += static_cast<char>(
                    toupper(static_cast<unsigned char>(ch)));
        }

        if (prefix.empty())
            prefix = "USER";

        if (prefix.length() > 12)
            prefix = prefix.substr(0, 12);

        return prefix;
    }

    void load()
    {
        ifstream fin(fileName.c_str());

        string prefix;
        int number;

        while (fin >> prefix >> number)
            lastNumber[prefix] = number;
    }

    void save() const
    {
        ofstream fout(fileName.c_str());

        for (const auto& item : lastNumber)
            fout << item.first << " "
                 << item.second << '\n';
    }

public:
    CarIDGenerator(const string& file = "car_id_registry.txt")
        : fileName(file)
    {
        load();
    }

    string generate(const string& userName)
    {
        string prefix = makePrefix(userName);

        int nextNumber = ++lastNumber[prefix];

        save();

        string numberText = to_string(nextNumber);

        while (numberText.length() < 3)
            numberText = "0" + numberText;

        return prefix + numberText;
    }
};

// ============================================================
// 3. CAR CLASS
// ============================================================

class Car
{
private:
    string carID;
    string ownerName;
    double batteryCapacity;
    double batteryPercentage;
    double consumptionRate;
    string currentLocation;
    string destination;

public:
    Car() : batteryCapacity(0),
            batteryPercentage(0),
            consumptionRate(DEFAULT_CONSUMPTION) {}

    void setDetails(const string& id, const string& owner,
                    double capacity, double percentage,
                    const string& current, const string& dest)
    {
        if (capacity <= 0)
            throw invalid_argument("Battery capacity must be greater than zero.");

        if (percentage < 0 || percentage > 100)
            throw invalid_argument("Battery percentage must be between 0 and 100.");

        if (current.empty() || dest.empty())
            throw invalid_argument("Locations cannot be empty.");

        carID = id;
        ownerName = owner;
        batteryCapacity = capacity;
        batteryPercentage = percentage;
        currentLocation = current;
        destination = dest;
        consumptionRate = DEFAULT_CONSUMPTION;
    }

    string getID() const { return carID; }
    string getOwner() const { return ownerName; }
    string getCurrentLocation() const { return currentLocation; }
    string getDestination() const { return destination; }

    double getBatteryCapacity() const { return batteryCapacity; }
    double getBatteryPercentage() const { return batteryPercentage; }
    double getConsumptionRate() const { return consumptionRate; }

    double remainingEnergy() const
    {
        return batteryCapacity * batteryPercentage / 100.0;
    }

    double remainingRange() const
    {
        return remainingEnergy() / consumptionRate;
    }

    void updateBattery(double percentage)
    {
        if (percentage < 0 || percentage > 100)
            throw invalid_argument("Invalid battery percentage.");

        batteryPercentage = percentage;
    }

    void display() const
    {
        cout << "\n============== VEHICLE DETAILS ==============\n";
        cout << "Owner Name                : " << ownerName << '\n';
        cout << "Automatically Generated ID: " << carID << '\n';
        cout << "Battery Capacity          : " << batteryCapacity << " kWh\n";
        cout << "Current Battery           : " << batteryPercentage << "%\n";
        cout << "Current Location          : " << currentLocation << '\n';
        cout << "Destination               : " << destination << '\n';
        cout << "Assumed Consumption       : " << consumptionRate << " kWh/km\n";

        cout << fixed << setprecision(2);

        cout << "Remaining Energy          : "
             << remainingEnergy() << " kWh\n";

        cout << "Estimated Range           : "
             << remainingRange() << " km\n";

        cout << "==============================================\n";
    }
};

// ============================================================
// 4. LINKED-LIST QUEUE FOR WAITING CARS
// ============================================================

class LinkedQueue
{
private:
    struct Node
    {
        string carID;
        Node* next;

        Node(const string& id) : carID(id), next(NULL) {}
    };

    Node* frontNode;
    Node* rearNode;
    int count;

public:
    LinkedQueue() : frontNode(NULL), rearNode(NULL), count(0) {}

    LinkedQueue(const LinkedQueue& other)
        : frontNode(NULL), rearNode(NULL), count(0)
    {
        Node* temp = other.frontNode;

        while (temp != NULL)
        {
            enqueue(temp->carID);
            temp = temp->next;
        }
    }

    LinkedQueue& operator=(const LinkedQueue& other)
    {
        if (this != &other)
        {
            clear();

            Node* temp = other.frontNode;

            while (temp != NULL)
            {
                enqueue(temp->carID);
                temp = temp->next;
            }
        }

        return *this;
    }

    void enqueue(const string& id)
    {
        Node* newNode = new Node(id);

        if (frontNode == NULL)
            frontNode = rearNode = newNode;
        else
        {
            rearNode->next = newNode;
            rearNode = newNode;
        }

        count++;
    }

    string dequeue()
    {
        if (frontNode == NULL)
            throw runtime_error("Waiting queue is empty.");

        Node* temp = frontNode;
        string id = temp->carID;

        frontNode = frontNode->next;

        if (frontNode == NULL)
            rearNode = NULL;

        delete temp;
        count--;

        return id;
    }

    bool empty() const { return frontNode == NULL; }
    int size() const { return count; }

    bool contains(const string& id) const
    {
        Node* temp = frontNode;

        while (temp != NULL)
        {
            if (temp->carID == id)
                return true;

            temp = temp->next;
        }

        return false;
    }

    bool remove(const string& id)
    {
        Node* current = frontNode;
        Node* previous = NULL;

        while (current != NULL)
        {
            if (current->carID == id)
            {
                if (previous == NULL)
                    frontNode = current->next;
                else
                    previous->next = current->next;

                if (current == rearNode)
                    rearNode = previous;

                delete current;
                count--;

                return true;
            }

            previous = current;
            current = current->next;
        }

        return false;
    }

    void display() const
    {
        Node* temp = frontNode;

        if (temp == NULL)
            cout << "None";

        while (temp != NULL)
        {
            cout << temp->carID;

            if (temp->next != NULL)
                cout << " -> ";

            temp = temp->next;
        }
    }

    void clear()
    {
        while (frontNode != NULL)
        {
            Node* temp = frontNode;
            frontNode = frontNode->next;
            delete temp;
        }

        rearNode = NULL;
        count = 0;
    }

    ~LinkedQueue()
    {
        clear();
    }
};

// ============================================================
// 5. CHARGER MANAGEMENT
// ============================================================

class Charger
{
private:
    int chargerID;
    string location;
    int totalSlots;
    double chargingPower;
    vector<string> chargingCars;
    LinkedQueue waitingCars;

public:
    Charger(int id, const string& place, int slots, double power)
        : chargerID(id), location(place),
          totalSlots(slots), chargingPower(power) {}

    int getID() const { return chargerID; }
    string getLocation() const { return location; }

    int getTotalSlots() const { return totalSlots; }

    int getChargingCount() const
    {
        return static_cast<int>(chargingCars.size());
    }

    int getAvailableSlots() const
    {
        return totalSlots - static_cast<int>(chargingCars.size());
    }

    int getWaitingCount() const
    {
        return waitingCars.size();
    }

    double getChargingPower() const
    {
        return chargingPower;
    }

    bool containsCar(const string& id) const
    {
        if (waitingCars.contains(id))
            return true;

        for (const string& current : chargingCars)
        {
            if (current == id)
                return true;
        }

        return false;
    }

    bool isCharging(const string& id) const
    {
        for (const string& current : chargingCars)
        {
            if (current == id)
                return true;
        }

        return false;
    }

    void assignWaitingCars()
    {
        while (getAvailableSlots() > 0 && !waitingCars.empty())
        {
            string id = waitingCars.dequeue();
            chargingCars.push_back(id);

            cout << id << " has started charging at "
                 << location << ".\n";
        }
    }

    void addCar(const string& id)
    {
        if (containsCar(id))
            throw runtime_error("This car is already at this charger.");

        if (getAvailableSlots() > 0)
        {
            chargingCars.push_back(id);

            cout << id << " assigned a charging slot at "
                 << location << ".\n";
        }
        else
        {
            waitingCars.enqueue(id);

            cout << "All slots are occupied. "
                 << id << " added to the waiting queue.\n";
        }
    }

    void finishCharging(const string& id)
    {
        for (int i = 0; i < static_cast<int>(chargingCars.size()); i++)
        {
            if (chargingCars[i] == id)
            {
                chargingCars.erase(chargingCars.begin() + i);

                cout << id << " completed charging at "
                     << location << ".\n";

                assignWaitingCars();
                return;
            }
        }

        throw runtime_error("This car is not currently charging.");
    }

    void removeWaitingCar(const string& id)
    {
        if (!waitingCars.remove(id))
            throw runtime_error("Car not found in the waiting queue.");

        cout << id << " removed from the waiting queue.\n";

        assignWaitingCars();
    }

    void display() const
    {
        cout << "\n--------------------------------------------\n";
        cout << "Charger ID          : CS" << chargerID << '\n';
        cout << "Location            : " << location << '\n';
        cout << "Total Slots         : " << totalSlots << '\n';
        cout << "Available Slots     : " << getAvailableSlots() << '\n';

        cout << "Cars Charging       : ";

        if (chargingCars.empty())
            cout << "None";
        else
        {
            for (const string& id : chargingCars)
                cout << id << " ";
        }

        cout << "\nCars Waiting        : ";
        waitingCars.display();

        cout << "\nCharging Power      : "
             << chargingPower << " kW\n";

        cout << "--------------------------------------------\n";
    }
};

// ============================================================
// 6. ROAD NETWORK, LOCATIONS AND DIJKSTRA
// ============================================================

struct Location
{
    string name;
    int chargerID;
};

struct Edge
{
    int source;
    int destination;
    double distance;
};

struct PathResult
{
    vector<double> distance;
    vector<int> parent;
};

class RoadNetwork
{
private:
    vector<Location> locations;
    vector<vector<pair<int, double>>> adj;

public:
    RoadNetwork()
    {
        locations = {
            {"Clock Tower", -1},
            {"ISBT", 2},
            {"Ballupur Chowk", -1},
            {"Chakrata Road", -1},
            {"Paltan Bazaar", -1},
            {"Railway Station", -1},
            {"Clement Town", 6},
            {"Prem Nagar", 5},
            {"Mussoorie Road", -1},
            {"Sahastradhara Road", 4},
            {"Pacific Mall", 1},
            {"Rajpur Road", 3},
            {"Sahastradhara", -1},
            {"FRI", 7},
            {"Forest Research Area", -1},
            {"Shimla Bypass", -1},
            {"Majra", -1},
            {"ITI Niranjanpur", -1},
            {"Sabzi Mandi Chowk", -1},
            {"Kamla Palace", -1},
            {"Engineers Enclave", -1},
            {"EPF Office", -1},
            {"Kanwali Road Chowk", -1},
            {"Balliwala Chowk", -1},
            {"Wadia Institute", -1},
            {"Patel Nagar Police Station", -1},
            {"Lal Pul", -1},
            {"Saharanpur Chowk", -1},
            {"Prince Chowk", -1},
            {"Cyber Police Station", -1},
            {"Tehsil Chowk", -1},
            {"Darshanlal Chowk", -1},
            {"Bindal Bridge", -1},
            {"Kishan Nagar Chowk", -1},
            {"Old Connaught Place", -1}
        };

        vector<Edge> edges = {
            {0,4,2.1}, {0,9,4.8}, {0,11,3.4}, {1,8,5.2},
            {2,3,2.8}, {2,10,3.1}, {3,8,4.5}, {4,6,3.2},
            {4,7,2.3}, {5,7,4.2}, {5,13,2.7}, {5,14,3.5},
            {9,12,1.5}, {11,9,4.0}, {11,12,3.0}, {13,14,1.2},
            {1,15,1.0}, {15,16,1.0}, {16,17,1.2}, {17,18,0.8},
            {18,25,0.8}, {25,26,0.8}, {26,27,1.0}, {27,5,0.8},
            {5,28,0.8}, {28,29,0.6}, {29,30,0.5}, {30,31,0.5},
            {31,0,0.6}, {17,19,0.8}, {19,20,0.6}, {20,21,0.5},
            {21,22,0.8}, {22,23,1.0}, {23,24,0.7}, {24,2,0.8},
            {2,13,1.2}, {0,34,0.7}, {34,32,0.7}, {32,33,0.8},
            {33,2,1.0}
        };

        adj.resize(locations.size());

        for (const Edge& edge : edges)
        {
            adj[edge.source].push_back(
                {edge.destination, edge.distance});

            adj[edge.destination].push_back(
                {edge.source, edge.distance});
        }
    }

    int size() const
    {
        return static_cast<int>(locations.size());
    }

    string getName(int index) const
    {
        return locations[index].name;
    }

    int findLocation(const string& input) const
    {
        string name = input;

        size_t first = name.find_first_not_of(" \t\r\n");

        if (first == string::npos)
            return -1;

        size_t last = name.find_last_not_of(" \t\r\n");

        name = name.substr(first, last - first + 1);

        bool numeric = true;

        for (char ch : name)
        {
            if (!isdigit(static_cast<unsigned char>(ch)))
                numeric = false;
        }

        if (numeric)
        {
            try
            {
                int index = stoi(name);

                if (index >= 0 && index < size())
                    return index;
            }
            catch (...) {}
        }

        string key;

        for (char ch : name)
        {
            key += static_cast<char>(
                tolower(static_cast<unsigned char>(ch)));
        }

        size_t annotation = key.find(" (charging station");

        if (annotation != string::npos)
            key = key.substr(0, annotation);

        size_t a = key.find_first_not_of(" \t\r\n");

        if (a == string::npos)
            return -1;

        size_t b = key.find_last_not_of(" \t\r\n");

        key = key.substr(a, b - a + 1);

        if (key == "niranjanpur mandi" || key == "niranjanpur")
            key = "iti niranjanpur";

        for (int i = 0; i < size(); i++)
        {
            string stored;

            for (char ch : locations[i].name)
            {
                stored += static_cast<char>(
                    tolower(static_cast<unsigned char>(ch)));
            }

            if (stored == key)
                return i;
        }

        return -1;
    }

    void displayLocations() const
    {
        cout << "\n--------------- AVAILABLE LOCATIONS ---------------\n";

        for (int i = 0; i < size(); i++)
        {
            cout << setw(2) << i << " : "
                 << locations[i].name;

            if (locations[i].chargerID != -1)
            {
                cout << " (Charging Station CS"
                     << locations[i].chargerID << ")";
            }

            cout << '\n';
        }
    }

    PathResult dijkstra(int source) const
    {
        PathResult result;

        vector<bool> visited(size(), false);

        result.distance.assign(size(), INF);
        result.parent.assign(size(), -1);

        priority_queue<
            pair<double, int>,
            vector<pair<double, int>>,
            greater<pair<double, int>>
        > pq;

        result.distance[source] = 0;

        pq.push({0, source});

        while (!pq.empty())
        {
            int u = pq.top().second;
            double currentDistance = pq.top().first;

            pq.pop();

            if (visited[u])
                continue;

            visited[u] = true;

            for (const auto& edge : adj[u])
            {
                int v = edge.first;
                double weight = edge.second;

                if (!visited[v] &&
                    currentDistance + weight < result.distance[v])
                {
                    result.distance[v] = currentDistance + weight;
                    result.parent[v] = u;

                    pq.push({result.distance[v], v});
                }
            }
        }

        return result;
    }

    vector<int> getPath(int source, int destination,
                        const vector<int>& parent) const
    {
        vector<int> path;

        int current = destination;

        while (current != -1)
        {
            path.push_back(current);

            if (current == source)
                break;

            current = parent[current];
        }

        if (path.empty() || path.back() != source)
            return vector<int>();

        reverse(path.begin(), path.end());

        return path;
    }

    void displayRoute(int source, int destination,
                      const PathResult& result) const
    {
        if (result.distance[destination] == INF)
        {
            cout << "No route is available between these locations.\n";
            return;
        }

        vector<int> path = getPath(
            source, destination, result.parent);

        cout << "\nShortest Route: ";

        for (int i = 0; i < static_cast<int>(path.size()); i++)
        {
            cout << locations[path[i]].name;

            if (i + 1 < static_cast<int>(path.size()))
                cout << " -> ";
        }

        cout << "\nTotal Distance: "
             << fixed << setprecision(2)
             << result.distance[destination] << " km\n";
    }
};

// ============================================================
// 7. SMART CHARGER RANKING
// ============================================================

struct ChargerOption
{
    int chargerID;
    string name;
    double distanceToStation;
    double detour;
    double onwardDistance;
    int waitingCars;
    int availableSlots;
    double chargingPower;
    double score;
    double endpointProximity;
    bool onShortestRoute;
    bool fallbackRanking;
};

struct ChargerComparator
{
    bool operator()(const ChargerOption& a,
                    const ChargerOption& b) const
    {
        if (a.fallbackRanking && b.fallbackRanking)
        {
            if (a.endpointProximity != b.endpointProximity)
                return a.endpointProximity > b.endpointProximity;
        }

        return a.score < b.score;
    }
};

int findChargerIndex(const vector<Charger>& chargers, int chargerID)
{
    for (int i = 0; i < static_cast<int>(chargers.size()); i++)
    {
        if (chargers[i].getID() == chargerID)
            return i;
    }

    return -1;
}

bool carExistsAtAnyStation(const vector<Charger>& chargers,
                           const string& carID)
{
    for (const Charger& charger : chargers)
    {
        if (charger.containsCar(carID))
            return true;
    }

    return false;
}

// Suitability score calculation
double calculateScore(const ChargerOption& option)
{
    double score = 100.0 / (option.distanceToStation + 1.0);

    score += 60.0 / (option.waitingCars + 1.0);

    score += option.availableSlots * 12.0;

    score += option.chargingPower * 1.5;

    score += 30.0 / (option.detour + 1.0);

    if (option.onShortestRoute)
        score += 20.0;

    return score;
}

int recommendCharger(const Car& car,
                     const RoadNetwork& network,
                     const vector<Charger>& chargers)
{
    int source = network.findLocation(car.getCurrentLocation());

    int destination = network.findLocation(car.getDestination());

    if (source == -1 || destination == -1)
    {
        cout << "Location name not found.\n";
        return -1;
    }

    PathResult fromSource = network.dijkstra(source);

    if (fromSource.distance[destination] == INF)
    {
        cout << "No road route found between these locations.\n";
        return -1;
    }

    PathResult fromDestination = network.dijkstra(destination);

    vector<int> mainRoute = network.getPath(
        source, destination, fromSource.parent);

    vector<bool> onMainRoute(network.size(), false);

    for (int node : mainRoute)
        onMainRoute[node] = true;

    const double mainDistance = fromSource.distance[destination];

    const double currentRange = car.remainingRange();

    const double fullBatteryRange =
        car.getBatteryCapacity() / car.getConsumptionRate();

    priority_queue<
        ChargerOption,
        vector<ChargerOption>,
        ChargerComparator
    > preferred;

    priority_queue<
        ChargerOption,
        vector<ChargerOption>,
        ChargerComparator
    > fallback;

    double nearestStationDistance = INF;
    string nearestStationName;

    bool foundReachableButInsufficient = false;

    cout << "\n=============== JOURNEY DETAILS ===============\n";

    cout << "From                    : "
         << car.getCurrentLocation() << '\n';

    cout << "Destination             : "
         << car.getDestination() << '\n';

    cout << "Shortest journey        : "
         << fixed << setprecision(2)
         << mainDistance << " km\n";

    cout << "Current battery range   : "
         << currentRange << " km\n";

    cout << "Range after full charge : "
         << fullBatteryRange << " km\n";

    cout << "Preferred detour limit  : "
         << MAX_DETOUR_KM << " km\n";

    for (const Charger& charger : chargers)
    {
        int stationNode = network.findLocation(
            charger.getLocation());

        if (stationNode == -1)
            continue;

        double toStation = fromSource.distance[stationNode];

        double onward = fromDestination.distance[stationNode];

        if (toStation == INF || onward == INF)
            continue;

        if (toStation < nearestStationDistance)
        {
            nearestStationDistance = toStation;
            nearestStationName = charger.getLocation();
        }

        if (toStation > currentRange + 0.0001)
            continue;

        if (onward > fullBatteryRange + 0.0001)
        {
            foundReachableButInsufficient = true;
            continue;
        }

        double totalViaStation = toStation + onward;

        double detour = totalViaStation - mainDistance;

        if (detour < 0 && detour > -0.0001)
            detour = 0;

        bool onRoute = onMainRoute[stationNode];

        bool nearRoute =
            (detour <= MAX_DETOUR_KM + 0.0001);

        ChargerOption option;

        option.chargerID = charger.getID();
        option.name = charger.getLocation();

        option.distanceToStation = toStation;
        option.detour = detour;
        option.onwardDistance = onward;

        option.waitingCars = charger.getWaitingCount();
        option.availableSlots = charger.getAvailableSlots();
        option.chargingPower = charger.getChargingPower();

        option.onShortestRoute = onRoute;

        option.score = calculateScore(option);

        option.endpointProximity = min(toStation, onward);

        option.fallbackRanking = !(onRoute || nearRoute);

        if (onRoute || nearRoute)
            preferred.push(option);
        else
            fallback.push(option);
    }

    bool usingFallback = preferred.empty();

    priority_queue<
        ChargerOption,
        vector<ChargerOption>,
        ChargerComparator
    > ranking = usingFallback ? fallback : preferred;

    if (ranking.empty())
    {
        cout << "\nNo suitable charging station can be recommended.\n";

        if (!nearestStationName.empty())
        {
            cout << "Nearest station: " << nearestStationName
                 << " (" << nearestStationDistance << " km away).\n";

            if (nearestStationDistance > currentRange)
                cout << "Current battery range is insufficient.\n";
            else if (foundReachableButInsufficient)
                cout << "Onward journey exceeds the estimated full-battery range.\n";
        }

        cout << "No queue registration has been made.\n";

        return -1;
    }

    cout << "\n========== CHARGING STATION OPTIONS ==========\n";

    if (usingFallback)
    {
        cout << "No reachable station was found on the route or within "
             << MAX_DETOUR_KM << " km extra distance.\n";

        cout << "Showing suitable nearby alternatives instead.\n";
    }
    else
    {
        cout << "Showing stations on the shortest route or within the preferred "
             << MAX_DETOUR_KM << " km detour.\n";
    }

    cout << "Only stations reachable with the current battery are considered.\n";
    cout << "The onward journey is checked assuming charging to 100%.\n";

    priority_queue<
        ChargerOption,
        vector<ChargerOption>,
        ChargerComparator
    > displayQueue = ranking;

    int rank = 1;

    ChargerOption best;

    while (!displayQueue.empty())
    {
        ChargerOption current = displayQueue.top();

        displayQueue.pop();

        if (rank == 1)
            best = current;

        cout << "\nRank " << rank++ << " : "
             << current.name << " (CS"
             << current.chargerID << ")\n";

        if (current.onShortestRoute)
            cout << "Route Category   : On shortest route\n";
        else if (current.detour <= MAX_DETOUR_KM + 0.0001)
            cout << "Route Category   : Nearby - within preferred detour\n";
        else
            cout << "Route Category   : Nearby alternative - outside preferred detour\n";

        cout << "Distance to Stop : "
             << current.distanceToStation << " km\n";

        cout << "Distance onward  : "
             << current.onwardDistance << " km\n";

        cout << "Total via Station: "
             << current.distanceToStation + current.onwardDistance
             << " km\n";

        cout << "Extra Detour     : "
             << current.detour << " km\n";

        cout << "Waiting Cars     : "
             << current.waitingCars << '\n';

        cout << "Available Slots  : "
             << current.availableSlots << '\n';

        cout << "Charging Power   : "
             << current.chargingPower << " kW\n";

        cout << "Suitability Score: "
             << fixed << setprecision(2)
             << current.score << '\n';
    }

    cout << "\n=============== BEST STATION RECOMMENDATION ===============\n";

    cout << "Recommended Station : "
         << best.name << " (CS" << best.chargerID << ")\n";

    if (best.onShortestRoute)
        cout << "Station Category    : On shortest route\n";
    else if (best.detour <= MAX_DETOUR_KM + 0.0001)
        cout << "Station Category    : Nearby station with small detour\n";
    else
        cout << "Station Category    : Nearest suitable alternative (outside preferred detour)\n";

    cout << "Distance to Station : "
         << best.distanceToStation << " km\n";

    cout << "Distance onward     : "
         << best.onwardDistance << " km\n";

    cout << "Total distance via station: "
         << best.distanceToStation + best.onwardDistance << " km\n";

    cout << "Additional detour   : "
         << best.detour << " km\n";

    cout << "Waiting Cars        : "
         << best.waitingCars << '\n';

    cout << "Available Slots     : "
         << best.availableSlots << '\n';

    cout << "Charging Power      : "
         << best.chargingPower << " kW\n";

    cout << "Suitability Score   : "
         << best.score << '\n';

    int bestNode = network.findLocation(best.name);

    cout << "\n--------- SHORTEST ROUTE FROM YOUR LOCATION TO STATION ---------\n";

    network.displayRoute(source, bestNode, fromSource);

    cout << "\nAfter charging to an assumed 100%, estimated range is "
         << fullBatteryRange << " km; onward distance is "
         << best.onwardDistance << " km.\n";

    cout << "Note: Station and queue details are simulated. "
         << "A station with no free slot can still accept a waiting-queue entry.\n";

    return best.chargerID;
}

// ============================================================
// 8. VEHICLE INPUT
// ============================================================

bool addVehicle(vector<Car>& cars,
                const string& userName,
                CarIDGenerator& generator,
                const RoadNetwork& network)
{
    double capacity, percentage;
    string current, destination;

    cout << "\n========== ENTER YOUR VEHICLE DETAILS ==========\n";

    cout << "Battery capacity (kWh): ";

    if (!(cin >> capacity) || capacity <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid battery capacity.\n";
        return false;
    }

    cout << "Current battery percentage: ";

    if (!(cin >> percentage) || percentage < 0 || percentage > 100)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid battery percentage.\n";
        return false;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    network.displayLocations();

    cout << "\nEnter your current location (name, number, or full station label): ";
    getline(cin, current);

    cout << "Enter your destination (name, number, or full station label): ";
    getline(cin, destination);

    int currentIndex = network.findLocation(current);

    int destinationIndex = network.findLocation(destination);

    if (currentIndex == -1 || destinationIndex == -1)
    {
        cout << "Invalid location. Please select a listed location.\n";
        return false;
    }

    current = network.getName(currentIndex);

    destination = network.getName(destinationIndex);

    if (currentIndex == destinationIndex)
    {
        cout << "Current location and destination cannot be the same.\n";
        return false;
    }

    string generatedID = generator.generate(userName);

    Car newCar;

    newCar.setDetails(
        generatedID,
        userName,
        capacity,
        percentage,
        current,
        destination
    );

    cars.push_back(newCar);

    cout << "\nVehicle details saved successfully.\n";

    cout << "Automatically generated Car ID: "
         << generatedID << '\n';

    cout << "Estimated remaining range: "
         << fixed << setprecision(2)
         << newCar.remainingRange() << " km\n";

    return true;
}

// ============================================================
// 9. USER FEEDBACK SYSTEM
// ============================================================

void collectFeedback(const string& userName)
{
    int rating;
    string note;

    cout << "\n====================================================\n";
    cout << "                  USER FEEDBACK                    \n";
    cout << "====================================================\n";

    cout << "\nHow would you rate your experience?\n";

    cout << "1. *     (1 Star)\n";
    cout << "2. **    (2 Stars)\n";
    cout << "3. ***   (3 Stars)\n";
    cout << "4. ****  (4 Stars)\n";
    cout << "5. ***** (5 Stars)\n";

    while (true)
    {
        cout << "\nEnter your rating (1-5): ";

        if (cin >> rating && rating >= 1 && rating <= 5)
            break;

        cout << "Invalid rating! Please enter a number from 1 to 5.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nWrite your feedback note (press Enter to skip): ";
    getline(cin, note);

    ofstream fout("feedback.txt", ios::app);

    if (!fout)
    {
        cout << "\nUnable to save feedback.\n";
        return;
    }

    fout << "Username: " << userName << '\n';
    fout << "Rating: " << rating << "/5 stars\n";

    fout << "Feedback: ";

    if (note.empty())
        fout << "No additional comments\n";
    else
        fout << note << '\n';

    fout << "--------------------------------------------\n";

    fout.close();

    cout << "\nThank you for your valuable feedback!\n";
    cout << "Feedback saved successfully.\n";

    cout << "\nYour Rating: ";

    for (int i = 0; i < rating; i++)
        cout << "* ";

    cout << "(" << rating << "/5)\n";
}

// ============================================================
// 10. MAIN FUNCTION
// ============================================================

int main()
{
    try
    {
        RoadNetwork network;

        CarIDGenerator generator;

        UserAuth auth;

        vector<Car> cars;

        vector<Charger> chargers;

        string userName;

        // Charging station data:
        // ID, location, total slots, charging power.

        chargers.push_back(Charger(1, "Pacific Mall", 4, 60));
        chargers.push_back(Charger(2, "ISBT", 3, 40));
        chargers.push_back(Charger(3, "Rajpur Road", 5, 50));
        chargers.push_back(Charger(4, "Sahastradhara Road", 4, 60));
        chargers.push_back(Charger(5, "Prem Nagar", 3, 30));
        chargers.push_back(Charger(6, "Clement Town", 2, 40));
        chargers.push_back(Charger(7, "FRI", 5, 50));

        // USER REGISTRATION / LOGIN

        bool loggedIn = auth.authenticate(userName);

        if (!loggedIn)
            return 0;

        // VEHICLE REGISTRATION

        char addAnother = 'y';

        while (addAnother == 'y' || addAnother == 'Y')
        {
            bool registered = addVehicle(
                cars, userName, generator, network);

            if (!registered)
            {
                cout << "\nWould you like to try entering "
                     << "the vehicle details again? (y/n): ";

                cin >> addAnother;

                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n');

                continue;
            }

            Car& currentCar = cars.back();

            cout << "\n\nHello, " << userName
                 << "! Finding the most suitable charging station...\n";

            int recommendedChargerID = recommendCharger(
                currentCar, network, chargers);

            if (recommendedChargerID != -1)
            {
                char joinQueue;

                cout << "\nWould you like to proceed to this "
                     << "recommended station and join its charging queue? (y/n): ";

                cin >> joinQueue;

                if (joinQueue == 'y' || joinQueue == 'Y')
                {
                    int chargerIndex = findChargerIndex(
                        chargers, recommendedChargerID);

                    if (chargerIndex != -1)
                    {
                        chargers[chargerIndex].addCar(
                            currentCar.getID());

                        cout << "Your vehicle "
                             << currentCar.getID()
                             << " has been registered at "
                             << chargers[chargerIndex].getLocation()
                             << ".\n";
                    }
                }
                else
                {
                    cout << "You have not been added to a charging queue.\n";
                }
            }

            cout << "\nWould you like to register another vehicle? (y/n): ";

            cin >> addAnother;

            cin.ignore(
                numeric_limits<streamsize>::max(), '\n');
        }

        // USER FEEDBACK AT THE END

        collectFeedback(userName);

        // FINAL MESSAGE

        cout << "\n====================================================\n";

        cout << "Thank you for using Smart EV Charging Spot Locator, "
             << userName << "!\n";

        cout << "Drive safe and have a great journey.\n";

        cout << "====================================================\n";
    }
    catch (const exception& e)
    {
        cout << "\nProgram error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
