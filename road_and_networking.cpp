/*
    ============================================================
        EV CHARGING SPOT LOCATOR ROAD NETWORKING MODULE
    ============================================================
    Author's Role:
    Road Network (Member 2)
    CURRENT CHECKPOINT:
    1. Create city locations
    2. Represent charging stations as special nodes
    3. Create weighted roads between locations
    4. Store the road network using an adjacency list

    Charging stations are represented only as SPECIAL NODES.
    Other charging-related functions will be integrated later
    by the respective team members.
    ============================================================
*/

#include <iostream>
#include <vector>  //A vector is like an array, but it can change its size while the program is running
                   //means it is used for dynamic size
#include <string>
#include <iomanip>//used to make the console output look properly aligned or formatted,
// especially for things like tables, distances, battery percentages, costs, etc.

using namespace std;

// ============================================================
// 1. LOCATION (VERTEX)
// ============================================================

struct Location //used to create our own data type
{
    string name;
    bool isChargingStation;  //This stores whether the location is a charging station or not. bool can have only two values: true false
    string stationId;
};


// Number of locations in the city constant means their value cant be changed
const int V = 15;


// ============================================================
// 2. CITY LOCATIONS
// ============================================================

vector<Location> locations = //This creates a vector called locations.<Location> The vector will contain objects of type Location.
{
    {"Clock Tower",          false, ""},
    {"ISBT",                 true,  "CS2"},
    {"Ballupur Chowk",       false, ""},
    {"Chakrata Road",        false, ""},
    {"Paltan Bazaar",        false, ""},
    {"Railway Station",      false, ""},
    {"Clement Town",         true,  "CS6"},
    {"Prem Nagar",           true,  "CS5"},
    {"Mussoorie Road",       false, ""},
    {"Sahastradhara Road",   true,  "CS4"},
    {"Pacific Mall",         true,  "CS1"},
    {"Rajpur Road",          true,  "CS3"},
    {"Sahastradhara",        false, ""},
    {"FRI",                  true,  "CS7"},
    {"Forest Research Area", false, ""}
}; //it has name ischargingstation and station id and now vector is initialized


// 3. EDGE (ROAD)

struct Edge
{
    int u;              // Starting location
    int v;              // Ending location
    double distance;    // Road distance in kilometres
};

// 4. CITY ROAD NETWORK

vector<Edge> edgeList =
{
    {0, 1, 2.6},{0, 2, 2.4}, {0, 4, 2.1},
    {0, 5, 3.0}, {0, 9, 4.8}, {0, 11, 3.4},
    {1, 8, 5.2}, {2, 3, 2.8}, {2, 10, 3.1},
    {3, 8, 4.5}, {4, 6, 3.2}, {4, 7, 2.3},
    {5, 7, 4.2}, {5, 13, 2.7}, {5, 14, 3.5},
    {9, 12, 1.5}, {11, 9, 4.0}, {11, 12, 3.0},{13, 14, 1.2}
};

// 5. ADJACENCY LIST
// adj[node] contains:
// {connected node, road distance}

vector<vector<pair<int, double>>> adj;

// 6. CREATE GRAPH

void createGraph()
{
    // Create an empty adjacency list
    adj.assign(V, {});


    // Add every road to the graph
    for (const auto &e : edgeList)
    {
        // Road from u to v
        adj[e.u].push_back(
            {e.v, e.distance}
        );

        // Road from v to u
        // because city roads are bidirectional
        adj[e.v].push_back(
            {e.u, e.distance}
        );
    }
}

// 7. DISPLAY CITY LOCATIONS

void displayLocations()
{
    cout << "\n";
    cout << "============================================================\n";
    cout << "                     CITY LOCATIONS\n";
    cout << "============================================================\n";

    for (int i = 0; i < V; i++)
    {
        cout << i << ". "
             << locations[i].name;

        if (locations[i].isChargingStation)
        {
            cout << " ["
                 << locations[i].stationId
                 << " - Charging Station]";
        }

        cout << "\n";
    }

    cout << "============================================================\n";
}

// 8. DISPLAY ROAD NETWORK


void displayRoadNetwork()
{
    cout << "\n";
    cout << "============================================================\n";
    cout << "                    CITY ROAD NETWORK\n";
    cout << "============================================================\n";

    for (const auto &e : edgeList)
    {
        cout << locations[e.u].name
             << "  <---- "
             << fixed
             << setprecision(1)
             << e.distance
             << " km ---->  "
             << locations[e.v].name
             << "\n";
    }

    cout << "============================================================\n";
}

// 9. DISPLAY ADJACENCY LIST

void displayAdjacencyList()
{
    cout << "\n";
    cout << "============================================================\n";
    cout << "                    ADJACENCY LIST\n";
    cout << "============================================================\n";

    for (int i = 0; i < V; i++)
    {
        cout << "\n[" << i << "] "
             << locations[i].name;

        if (locations[i].isChargingStation)
        {
            cout << " ("
                 << locations[i].stationId
                 << ")";
        }

        cout << "\n";

        for (const auto &connection : adj[i])
        {
            int neighbour = connection.first;
            double distance = connection.second;

            cout << "    -> "
                 << neighbour
                 << " ("
                 << locations[neighbour].name
                 << ", "
                 << fixed
                 << setprecision(1)
                 << distance
                 << " km)";
        }

        cout << "\n";
    }

    cout << "\n============================================================\n";
}

// 10. MENU


void showMenu()
{
    cout << "\n";
    cout << "============================================================\n";
    cout << "          EV CHARGING SPOT LOCATOR\n";
    cout << "             ROAD NETWORK MODULE\n";
    cout << "============================================================\n";

    cout << "1. Display City Locations\n";
    cout << "2. Display Road Network\n";
    cout << "3. Display Adjacency List\n";
    cout << "4. Exit\n";
}

// 11. MAIN

int main()
{
    // Build the graph
    createGraph();
    int choice;
    do
    {
        showMenu();
        cout << "\nEnter your choice: ";
        cin >> choice;
        switch (choice)
        {
            case 1:
                displayLocations();
                break;
            case 2:
                displayRoadNetwork();
                break;
            case 3:
                displayAdjacencyList();
                break;
            case 4:
                cout << "\nExiting Road Network Module.\n";
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }

    }
    while (choice != 4);
    return 0;
}