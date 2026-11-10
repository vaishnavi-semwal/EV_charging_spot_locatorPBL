#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

// Structure for storing charger information
struct Charger {
    string name;
    double distance;       // Distance in km
    int waitingCars;       // Cars already waiting
    int availableSlots;   // Free charging slots
    double chargingPower; // Charging power in kW
    double score;          // Calculated score
    bool canChargeFull;    // Can car reach and charge fully?
};

// --------------------------------------------------
// Function to calculate score
// Higher score = better charger
// --------------------------------------------------
double calculateScore(Charger c) {

    double score = 0;

    // Less distance is better
    score += (100.0 / (c.distance + 1));

    // Less waiting is better
    score += (50.0 / (c.waitingCars + 1));

    // More available slots are better
    score += c.availableSlots * 10;

    // Higher charging power is better
    score += c.chargingPower * 2;

    // Charger must be capable of charging the car fully
    if (c.canChargeFull)
        score += 30;

    return score;
}

// --------------------------------------------------
// Custom Comparator for Priority Queue
// Highest score comes first
// --------------------------------------------------
struct ChargerComparator {

    bool operator()(const Charger& a, const Charger& b) {

        return a.score < b.score;
    }
};

// --------------------------------------------------
// Function to check whether charger is reachable
// --------------------------------------------------
bool isReachable(double distance, double currentRange) {

    return distance <= currentRange;
}

// --------------------------------------------------
// Function to check whether charger can charge
// the vehicle to full
// --------------------------------------------------
bool canChargeToFull(
    double batteryCapacity,
    double currentBattery,
    double chargingPower
) {

    double requiredEnergy = batteryCapacity - currentBattery;

    if (requiredEnergy <= 0)
        return true;

    // Assume charger can provide enough energy
    // for the required charge
    return chargingPower > 0;
}

// --------------------------------------------------
// Function to display charger information
// --------------------------------------------------
void displayCharger(const Charger& c) {

    cout << "\n-----------------------------------";
    cout << "\nCharger: " << c.name;
    cout << "\nDistance: " << c.distance << " km";
    cout << "\nCars Waiting: " << c.waitingCars;
    cout << "\nAvailable Slots: " << c.availableSlots;
    cout << "\nCharging Power: " << c.chargingPower << " kW";

    cout << "\nCan Charge Fully: ";

    if (c.canChargeFull)
        cout << "Yes";
    else
        cout << "No";

    cout << "\nScore: " << fixed << setprecision(2)
         << c.score;

    cout << "\n-----------------------------------";
}

// --------------------------------------------------
// Main Function
// --------------------------------------------------
int main() {

    cout << "=============================================\n";
    cout << "       SMART EV CHARGER SELECTION\n";
    cout << "=============================================\n";

    // ---------------------------------------------
    // Vehicle input
    // ---------------------------------------------

    double batteryCapacity;
    double currentBattery;
    double currentRange;

    cout << "\nEnter battery capacity of vehicle (kWh): ";
    cin >> batteryCapacity;

    cout << "Enter current battery level (kWh): ";
    cin >> currentBattery;

    cout << "Enter current driving range (km): ";
    cin >> currentRange;

    // ---------------------------------------------
    // Number of chargers
    // ---------------------------------------------

    int n;

    cout << "\nEnter number of chargers: ";
    cin >> n;

    vector<Charger> chargers;

    // ---------------------------------------------
    // Input charger information
    // ---------------------------------------------

    for (int i = 0; i < n; i++) {

        Charger c;

        cout << "\n========== Charger " << i + 1 << " ==========\n";

        cout << "Enter charger name: ";
        cin >> c.name;

        cout << "Enter distance (km): ";
        cin >> c.distance;

        cout << "Enter cars waiting: ";
        cin >> c.waitingCars;

        cout << "Enter available slots: ";
        cin >> c.availableSlots;

        cout << "Enter charging power (kW): ";
        cin >> c.chargingPower;

        // Check whether charger is reachable
        if (!isReachable(c.distance, currentRange)) {

            cout << "\n" << c.name
                 << " is NOT reachable.\n";

            continue;
        }

        // Check whether charger can charge vehicle
        c.canChargeFull = canChargeToFull(
            batteryCapacity,
            currentBattery,
            c.chargingPower
        );

        // Calculate score
        c.score = calculateScore(c);

        // Store only reachable chargers
        chargers.push_back(c);
    }

    // ---------------------------------------------
    // Check if any charger is reachable
    // ---------------------------------------------

    if (chargers.empty()) {

        cout << "\n=============================================\n";
        cout << "No reachable charger found.\n";
        cout << "Please charge your vehicle using another option.\n";
        cout << "=============================================\n";

        return 0;
    }

    // ---------------------------------------------
    // Display reachable chargers
    // ---------------------------------------------

    cout << "\n\n=============================================\n";
    cout << "          REACHABLE CHARGERS\n";
    cout << "=============================================\n";

    for (const Charger& c : chargers) {

        displayCharger(c);
    }

    // ---------------------------------------------
    // Sorting chargers according to score
    // ---------------------------------------------

    sort(
        chargers.begin(),
        chargers.end(),
        [](const Charger& a, const Charger& b) {

            return a.score > b.score;
        }
    );

    // ---------------------------------------------
    // Priority Queue
    // ---------------------------------------------

    priority_queue<
        Charger,
        vector<Charger>,
        ChargerComparator
    > pq;

    // Insert all reachable chargers into priority queue
    for (const Charger& c : chargers) {

        pq.push(c);
    }

    // ---------------------------------------------
    // Best charger
    // ---------------------------------------------

    Charger bestCharger = pq.top();

    // ---------------------------------------------
    // Display ranking
    // ---------------------------------------------

    cout << "\n\n=============================================\n";
    cout << "          CHARGER RANKING\n";
    cout << "=============================================\n";

    for (int i = 0; i < chargers.size(); i++) {

        cout << "\nRank " << i + 1;

        cout << "\nCharger: " << chargers[i].name;

        cout << "\nScore: "
             << fixed << setprecision(2)
             << chargers[i].score;

        cout << "\nDistance: "
             << chargers[i].distance << " km";

        cout << "\nWaiting Cars: "
             << chargers[i].waitingCars;

        cout << "\nAvailable Slots: "
             << chargers[i].availableSlots;

        cout << "\nCharging Power: "
             << chargers[i].chargingPower << " kW";
    }

    // ---------------------------------------------
    // Final Recommendation
    // ---------------------------------------------

    cout << "\n\n=============================================\n";
    cout << "       FINAL RECOMMENDATION\n";
    cout << "=============================================\n";

    cout << "\nRecommended Charger: "
         << bestCharger.name;

    cout << "\nScore: "
         << fixed << setprecision(2)
         << bestCharger.score;

    cout << "\nDistance: "
         << bestCharger.distance << " km";

    cout << "\nCars Waiting: "
         << bestCharger.waitingCars;

    cout << "\nAvailable Slots: "
         << bestCharger.availableSlots;

    cout << "\nCharging Power: "
         << bestCharger.chargingPower << " kW";

    // ---------------------------------------------
    // Reason for recommendation
    // ---------------------------------------------

    cout << "\n\nReason for Recommendation:\n";

    cout << "- Charger is reachable.\n";

    if (bestCharger.distance <= currentRange)
        cout << "- Distance is within the vehicle's available range.\n";

    if (bestCharger.waitingCars <= 2)
        cout << "- Low waiting time.\n";
    else
        cout << "- Waiting time is acceptable compared with other chargers.\n";

    if (bestCharger.availableSlots > 0)
        cout << "- Charging slot is available.\n";

    if (bestCharger.chargingPower >= 50)
        cout << "- Provides high charging power.\n";
    else
        cout << "- Provides suitable charging power.\n";

    cout << "- It received the highest overall score.\n";

    cout << "\n=============================================\n";
    cout << "        SMART CHARGER SELECTED\n";
    cout << "=============================================\n";

    return 0;
}
