
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
using namespace std;

const int V = 35;

struct Location
{
    string name;
    bool isChargingStation;
    int stationId;
};

struct Edge
{
    int src;
    int dest;
    double weight;
};

vector<Location> locations =
{
    {"Clock Tower", false, -1},
    {"ISBT", true, 2},
    {"Ballupur Chowk", false, -1},
    {"Chakrata Road", false, -1},
    {"Paltan Bazaar", false, -1},
    {"Railway Station", false, -1},
    {"Clement Town", true, 6},
    {"Prem Nagar", true, 5},
    {"Mussoorie Road", false, -1},
    {"Sahastradhara Road", true, 4},
    {"Pacific Mall", true, 1},
    {"Rajpur Road", true, 3},
    {"Sahastradhara", false, -1},
    {"FRI", true, 7},
    {"Forest Research Area", false, -1},

    {"Shimla Bypass", false, -1},
    {"Majra", false, -1},
    {"ITI Niranjanpur", false, -1},
    {"Sabzi Mandi Chowk", false, -1},
    {"Kamla Palace", false, -1},
    {"Engineers Enclave", false, -1},
    {"EPF Office", false, -1},
    {"Kanwali Road Chowk", false, -1},
    {"Balliwala Chowk", false, -1},
    {"Wadia Institute", false, -1},
    {"Patel Nagar Police Station", false, -1},
    {"Lal Pul", false, -1},
    {"Saharanpur Chowk", false, -1},
    {"Prince Chowk", false, -1},
    {"Cyber Police Station", false, -1},
    {"Tehsil Chowk", false, -1},
    {"Darshanlal Chowk", false, -1},
    {"Bindal Bridge", false, -1},
    {"Kishan Nagar Chowk", false, -1},
    {"Old Connaught Place", false, -1}
};

vector<Edge> edgeList =
{
    // Original road connections
    {0,4,2.1},
    {0,9,4.8},
    {0,11,3.4},
    {1,8,5.2},
    {2,3,2.8},
    {2,10,3.1},
    {3,8,4.5},
    {4,6,3.2},
    {4,7,2.3},
    {5,7,4.2},
    {5,13,2.7},
    {5,14,3.5},
    {9,12,1.5},
    {11,9,4.0},
    {11,12,3.0},
    {13,14,1.2},

    // ISBT towards Railway Station
    {1,15,1.0},
    {15,16,1.0},
    {16,17,1.2},
    {17,18,0.8},
    {18,25,0.8},
    {25,26,0.8},
    {26,27,1.0},
    {27,5,0.8},

    // Railway Station towards Clock Tower
    {5,28,0.8},
    {28,29,0.6},
    {29,30,0.5},
    {30,31,0.5},
    {31,0,0.6},

    // ISBT towards FRI through GMS Road
    {17,19,0.8},
    {19,20,0.6},
    {20,21,0.5},
    {21,22,0.8},
    {22,23,1.0},
    {23,24,0.7},
    {24,2,0.8},
    {2,13,1.2},

    // Clock Tower towards FRI through other junctions
    {0,34,0.7},
    {34,32,0.7},
    {32,33,0.8},
    {33,2,1.0}
};

vector<vector<pair<int,double>>> adj;

void createGraph()
{
    adj.assign(V, {});

    for(int i = 0; i < edgeList.size(); i++)
    {
        int u = edgeList[i].src;
        int v = edgeList[i].dest;
        double w = edgeList[i].weight;

        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }
}

void displayLocations()
{
    cout << "\n--------- LOCATIONS ---------\n";

    for(int i = 0; i < V; i++)
    {
        cout << i << " : " << locations[i].name;

        if(locations[i].isChargingStation)
        {
            cout << " (Charging Station CS"
                 << locations[i].stationId << ")";
        }

        cout << endl;
    }
}

void displayRoadNetwork()
{
    cout << "\n--------- ROAD NETWORK ---------\n";

    cout << left << setw(25) << "Source"
         << setw(25) << "Destination"
         << "Distance (km)\n";

    for(int i = 0; i < edgeList.size(); i++)
    {
        cout << left << setw(25)
             << locations[edgeList[i].src].name
             << setw(25)
             << locations[edgeList[i].dest].name
             << edgeList[i].weight << endl;
    }
}

void displayAdjacencyList()
{
    cout << "\n--------- ADJACENCY LIST ---------\n";

    for(int i = 0; i < V; i++)
    {
        cout << locations[i].name << " -> ";

        for(int j = 0; j < adj[i].size(); j++)
        {
            cout << locations[adj[i][j].first].name
                 << " (" << adj[i][j].second << " km)";

            if(j != adj[i].size()-1)
                cout << " , ";
        }

        cout << endl;
    }
}

void dijkstra(int source, vector<double>& dist,
              vector<int>& parent)
{
    vector<bool> visited(V, false);

    dist.assign(V, numeric_limits<double>::infinity());
    parent.assign(V, -1);

    dist[source] = 0;

    for(int i = 0; i < V; i++)
    {
        int u = -1;
        double minimum = numeric_limits<double>::infinity();

        for(int j = 0; j < V; j++)
        {
            if(!visited[j] && dist[j] < minimum)
            {
                minimum = dist[j];
                u = j;
            }
        }

        if(u == -1)
            break;

        visited[u] = true;

        for(int j = 0; j < adj[u].size(); j++)
        {
            int v = adj[u][j].first;
            double weight = adj[u][j].second;

            if(!visited[v] &&
               dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                parent[v] = u;
            }
        }
    }
}

void displayShortestRoute(int source, int destination,
                          vector<double>& dist,
                          vector<int>& parent)
{
    if(dist[destination] == numeric_limits<double>::infinity())
    {
        cout << "\nNo route available between these locations.\n";
        return;
    }

    vector<int> path;

    int current = destination;

    while(current != -1)
    {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    cout << "\n--------- SHORTEST ROUTE ---------\n";

    for(int i = 0; i < path.size(); i++)
    {
        cout << locations[path[i]].name;

        if(i != path.size()-1)
            cout << " -> ";
    }

    cout << "\nTotal Distance: "
         << fixed << setprecision(2)
         << dist[destination] << " km\n";
}

void findShortestPath()
{
    int source, destination;

    displayLocations();

    cout << "\nEnter source location index: ";
    cin >> source;

    cout << "Enter destination location index: ";
    cin >> destination;

    if(source < 0 || source >= V ||
       destination < 0 || destination >= V)
    {
        cout << "\nInvalid location index!\n";
        return;
    }

    vector<double> dist;
    vector<int> parent;

    dijkstra(source, dist, parent);

    displayShortestRoute(source, destination, dist, parent);
}

void findNearestChargingStation()
{
    int source;

    displayLocations();

    cout << "\nEnter your current location index: ";
    cin >> source;

    if(source < 0 || source >= V)
    {
        cout << "\nInvalid location index!\n";
        return;
    }

    vector<double> dist;
    vector<int> parent;

    dijkstra(source, dist, parent);

    int nearest = -1;
    double minimum = numeric_limits<double>::infinity();

    for(int i = 0; i < V; i++)
    {
        if(locations[i].isChargingStation &&
           dist[i] < minimum)
        {
            minimum = dist[i];
            nearest = i;
        }
    }

    if(nearest == -1)
    {
        cout << "\nNo charging station is reachable.\n";
        return;
    }

    cout << "\nNearest Charging Station: "
         << locations[nearest].name;

    cout << " (CS" << locations[nearest].stationId << ")";

    cout << "\nDistance: " << fixed << setprecision(2)
         << minimum << " km\n";

    displayShortestRoute(source, nearest, dist, parent);
}

void showMenu()
{
    cout << "\n========== EV CHARGING SPOT LOCATOR ==========\n";
    cout << "1. Display All Locations\n";
    cout << "2. Display Road Network\n";
    cout << "3. Display Adjacency List\n";
    cout << "4. Find Shortest Route\n";
    cout << "5. Find Nearest Charging Station\n";
    cout << "6. Exit\n";
    cout << "Enter your choice: ";
}

int main()
{
    createGraph();

    int choice;

    do
    {
        showMenu();
        cin >> choice;

        switch(choice)
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
                findShortestPath();
                break;

            case 5:
                findNearestChargingStation();
                break;

            case 6:
                cout << "\nExiting program...\n";
                break;

            default:
                cout << "\nInvalid choice! Try again.\n";
        }

    }while(choice != 6);

    return 0;
}
