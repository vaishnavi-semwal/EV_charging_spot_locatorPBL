/*
    ============================================================
        EV CHARGING SPOT LOCATOR ROAD NETWORKING MODULE
    ============================================================
    Author's Role:
    managing EV charging station (Member 4)
    Member 4 manages EV charging station details such as
    station ID, name, and location.
    They also display whether each charging station is 
    currently available or unavailable.
    ============================================================
*/
#include <iostream>
#include <vector>
using namespace std;

struct ChargingStation
{
    int id;
    string name;
    string location;
    bool available;
};

int main()
{
    vector<ChargingStation> stations;
stations.push_back({1, "CS1", "Pacific Mall", true});
stations.push_back({2, "CS2", "ISBT", true});
stations.push_back({3, "CS3", "Rajpur Road", true});
stations.push_back({4, "CS4", "Sahastradhara Road", true});
stations.push_back({5, "CS5", "Prem Nagar", true});
stations.push_back({6, "CS6", "Clement Town", true});
stations.push_back({7, "CS7", "FRI", true});
   
    cout << "EV Charging Stations\n\n";

    for (auto station : stations)
    {
        cout << "ID: " << station.id << endl;
        cout << "Name: " << station.name << endl;
        cout << "Location: " << station.location << endl;
        cout << "Available: "
             << (station.available ? "Yes" : "No") << endl;
        cout << "----------------------\n";
    }

    return 0;
}
