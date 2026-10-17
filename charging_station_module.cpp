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
/*
    ============================================================
        EV CHARGING SPOT LOCATOR ROAD NETWORKING MODULE
    ============================================================
    Module 4: EV and battery information
    ============================================================
*/

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Car
{
private:
    string carId;
    double batteryCapacity;
    double currentBatteryPercentage;
    double energyConsumptionRate;
    string currentLocation;

public:
    Car(string id, double capacity, double batteryPercentage,
        double consumptionRate, string location)
    {
        carId = id;
        batteryCapacity = capacity;
        energyConsumptionRate = consumptionRate;
        currentLocation = location;

        setBatteryPercentage(batteryPercentage);
    }

    void setBatteryPercentage(double percentage)
    {
        if (percentage >= 0 && percentage <= 100)
        {
            currentBatteryPercentage = percentage;
        }
        else
        {
            currentBatteryPercentage = 0;
            cout << "Invalid battery percentage. Set to 0%.\n";
        }
    }

    double calculateRemainingEnergy() const
    {
        return batteryCapacity * currentBatteryPercentage / 100;
    }

    double calculateEstimatedRange() const
    {
        if (energyConsumptionRate <= 0)
        {
            return 0;
        }

        return calculateRemainingEnergy() / energyConsumptionRate;
    }

    void displayInformation() const
    {
        cout << "EV and Battery Information\n";
        cout << "Car ID: " << carId << endl;
        cout << "Battery Capacity: " << batteryCapacity << " kWh\n";
        cout << "Current Battery: " << currentBatteryPercentage << "%\n";
        cout << "Energy Consumption Rate: "
             << energyConsumptionRate << " kWh/km\n";
        cout << "Current Location: " << currentLocation << endl;
        cout << "Remaining Energy: "
             << calculateRemainingEnergy() << " kWh\n";
        cout << "Estimated Range: "
             << calculateEstimatedRange() << " km\n";
    }
};

struct ChargingStation
{
    int id;
    string name;
    string location;
    bool available;
};

int main()
{
    Car car("EV101", 60, 35, 0.15, "GEHU");

    vector<ChargingStation> stations;

    stations.push_back({1, "CS1", "Pacific Mall", true});
    stations.push_back({2, "CS2", "ISBT", true});
    stations.push_back({3, "CS3", "Rajpur Road", true});
    stations.push_back({4, "CS4", "Sahastradhara Road", true});
    stations.push_back({5, "CS5", "Prem Nagar", true});
    stations.push_back({6, "CS6", "Clement Town", true});
    stations.push_back({7, "CS7", "FRI", true});

    car.displayInformation();

    cout << "\nEV Charging Stations\n\n";

    for (const auto& station : stations)
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
