#include <iostream>
#include <string>

using namespace std;

// Class for managing EV and battery information
class Car {
private:
    string carId;
    double batteryCapacity;
    double currentBatteryPercentage;
    double energyConsumptionRate;
    string currentLocation;

public:

    // Parameterized constructor
    Car(string id, double capacity, double percentage,
        double consumption, string location) {

        carId = id;
        batteryCapacity = capacity;
        setBatteryPercentage(percentage);
        energyConsumptionRate = consumption;
        currentLocation = location;
    }

    // Validate and set battery percentage
    void setBatteryPercentage(double percentage) {

        if (percentage >= 0 && percentage <= 100) {
            currentBatteryPercentage = percentage;
        }
        else {
            cout << "Invalid battery percentage!" << endl;
            currentBatteryPercentage = 0;
        }
    }

    // Calculate remaining battery energy
    double calculateRemainingEnergy() {

        return (batteryCapacity * currentBatteryPercentage) / 100;
    }

    // Calculate estimated driving range
    double calculateRemainingRange() {

        if (energyConsumptionRate <= 0) {
            return 0;
        }

        return calculateRemainingEnergy() / energyConsumptionRate;
    }

    // Display EV information
    void displayCarStatus() {

        cout << "\n----- EV STATUS -----" << endl;

        cout << "Car ID: " << carId << endl;
        cout << "Location: " << currentLocation << endl;
        cout << "Battery Capacity: "
             << batteryCapacity << " kWh" << endl;

        cout << "Battery Remaining: "
             << currentBatteryPercentage << "%" << endl;

        cout << "Energy Consumption: "
             << energyConsumptionRate << " kWh/km" << endl;

        cout << "Remaining Energy: "
             << calculateRemainingEnergy() << " kWh" << endl;

        cout << "Estimated Range: "
             << calculateRemainingRange() << " km" << endl;
    }
};


int main() {

    Car myCar(
        "EV-001",
        60,
        35,
        0.15,
        "Graphic Era Hill University"
    );

    myCar.displayCarStatus();

    return 0;
}