#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;

class Car
{
private:
    double batteryCapacity;
    double batteryPercentage;
    double energyConsumptionRate;

public:
    Car(double capacity, double percentage, double consumption)
    {
        batteryCapacity = capacity;
        energyConsumptionRate = consumption;

        if (percentage >= 0 && percentage <= 100)
            batteryPercentage = percentage;
        else
        {
            batteryPercentage = 0;
            cout << "Invalid battery percentage. Set to 0%.\n";
        }
    }

    double getRange()
    {
        if (energyConsumptionRate <= 0)
            return 0;

        return (batteryCapacity * batteryPercentage / 100)
               / energyConsumptionRate;
    }
};

struct Charger
{
    string name;
    double distance;
    int carsWaiting;
    int availableSlots;
    double chargingPower;
    bool canChargeToFull;
    double score;
};

struct CompareCharger
{
    bool operator()(const Charger& a, const Charger& b)
    {
        return a.score > b.score; // Lowest score comes first
    }
};

int main()
{
    Car car(60, 35, 0.15);

    vector<Charger> chargers = {
        {"Charger A", 15, 8, 2, 50, true, 0},
        {"Charger B", 22, 1, 4, 60, true, 0},
        {"Charger C", 30, 0, 3, 40, true, 0}
    };

    double range = car.getRange();
    priority_queue<Charger, vector<Charger>, CompareCharger> ranking;

    for (Charger charger : chargers)
    {
        if (charger.distance <= range &&
            charger.availableSlots > 0 &&
            charger.canChargeToFull)
        {
            charger.score = charger.distance * 2
                          + charger.carsWaiting * 5
                          - charger.availableSlots * 10
                          - charger.chargingPower * 0.5;

            ranking.push(charger);
        }
    }

    if (ranking.empty())
    {
        cout << "No suitable reachable charger found.\n";
        return 0;
    }

    Charger bestCharger = ranking.top();

    cout << "Estimated car range: " << range << " km\n";
    cout << "Recommended charger: " << bestCharger.name << endl;
    cout << "Distance: " << bestCharger.distance << " km\n";
    cout << "Cars waiting: " << bestCharger.carsWaiting << endl;
    cout << "Available slots: " << bestCharger.availableSlots << endl;
    cout << "Charging power: " << bestCharger.chargingPower << " kW\n";
    cout << "Reason: This charger has the best score among "
         << "reachable suitable chargers.\n";

    return 0;
}
