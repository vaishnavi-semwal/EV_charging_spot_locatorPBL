/*
    ============================================================
        EV CHARGING SPOT LOCATOR ROAD NETWORKING MODULE
    ============================================================
    Author's Role: Member 1: Road networking 
                   Member 2:
                   Member 3: 
                   Member 4: Charging station
    Charging stations are represented only as SPECIAL NODES.
    Other charging-related functions will be integrated later
    by the respective team members.
    ============================================================
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;


// ================================================================
//=================================================================
//               ROAD NETWORK AND ROUTTING : MEMBER 1
// ================================================================
//=================================================================
//  LOCATION (VERTEX)
struct Location
{
    string name;
    bool isChargingStation;
    string stationId;
};

// Number of locations in the city
const int V = 15;

//  CITY LOCATIONS

vector<Location> locations =
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
};


//  EDGE (ROAD)

struct Edge
{
    int u;              // Starting location
    int v;              // Ending location
    double distance;    // Road distance in kilometres
};

//  CITY ROAD NETWORK

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

//  CREATE GRAPH

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

//  DISPLAY CITY LOCATIONS

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

//  DISPLAY ROAD NETWORK


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

//  DISPLAY ADJACENCY LIST

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





//=================================================================
//=================================================================
//               CHARGING STATION : MEMBER 4
// ================================================================
//=================================================================





// 8. SEARCH CHARGING STATION

void searchChargingStation()
{
    string id;
    bool found = false;

    cout << "\nEnter Charging Station ID: ";
    cin >> id;

    for (const auto &location : locations)
    {
        if (location.isChargingStation &&
            location.stationId == id)
        {
            cout << "\nCharging Station Found!\n";
            cout << "Station ID : " << location.stationId << endl;
            cout << "Location   : " << location.name << endl;

            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "\nCharging Station not found.\n";
    }
}




// MENU


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
    cout << "4. Search Charging Station\n";    // MEMBER 4
    cout << "5. Exit\n";
}
//==============================================================================
//=============================================================================

// MAIN

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
