/*
    ============================================================
    EV CHARGING SPOT LOCATOR - ROAD NETWORKING MODULE
    ============================================================
    Author's Role: Road Network & Routing (Member 2)
    Scope: Graph creation, Dijkstra's shortest path, and a  console MAP-STYLE
           visualization of the city network with the shortest route highlighted in green.
    NOT included here (handled by teammates):
    - Battery / range calculation
    - Charging station availability / waiting cars
    - Charging logic
    Charging stations only appear here as SPECIAL NODES so the
    rest of the team can plug their logic into them later.
    ------------------------------------------------------------
    A NOTE ON THE MAP DRAWING (read this before the viva!):
    A plain text console has no coordinate system - it is just
    rows and columns of characters. There is no general way to
    auto-arrange an arbitrary graph into a 2D picture (that is
    a real, hard problem called "graph layout", solved by tools
    like Graphviz using serious algorithms).
    What THIS program does instead: each of the 15 locations is
    given a FIXED (row, column) position on a text canvas, hand-
    chosen to roughly match the real city map. Roads are then
    drawn as straight or diagonal character lines between those
    fixed points. This only works because the 15 positions were
    chosen specifically for THIS graph - it is not a generic
    graph-drawing algorithm.
    ============================================================
*/

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <climits>
#include <iomanip>
#include <limits>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

using namespace std;

// ------------------------------------------------------------
// 1. DATA: LOCATIONS (NODES)
// ------------------------------------------------------------
struct Location {
    string name;       // full name, used in menus and route text
    string shortLabel; // short name, used on the map canvas
    bool isChargingStation;
    string stationId;
    int row, col;       // fixed position on the map canvas
};

const int V = 15;

vector<Location> locations = {
    {"Clock Tower",          "ClockTower",   false, "", 20, 50},  // 0
    {"ISBT",                 "ISBT",         true,  "CS2", 10, 50}, // 1
    {"Ballupur Chowk",       "Ballupur",     false, "", 14, 32},  // 2
    {"Chakrata Road",        "Chakrata",     false, "", 6, 28},   // 3
    {"Paltan Bazaar",        "PaltanBzr",    false, "", 28, 50},  // 4
    {"Railway Station",      "RailwaySt",    false, "", 24, 25},  // 5
    {"Clement Town",         "ClementTn",    true,  "CS6", 34, 65}, // 6
    {"Prem Nagar",           "PremNagar",    true,  "CS5", 34, 40}, // 7
    {"Mussoorie Road",       "Mussoorie",    false, "", 2, 50},   // 8
    {"Sahastradhara Road",   "SahasRd",      true,  "CS4", 10, 75}, // 9
    {"Pacific Mall",         "PacificMall",  true,  "CS1", 14, 6}, // 10
    {"Rajpur Road",          "RajpurRd",     true,  "CS3", 20, 80}, // 11
    {"Sahastradhara",        "Sahastrdhr",   false, "", 14, 95},  // 12
    {"FRI",                  "FRI",          true,  "CS7", 30, 8}, // 13
    {"Forest Research Area", "ForestArea",   false, "", 34, 6}    // 14
};

// ------------------------------------------------------------
// 2. DATA: ROADS (EDGES) - {from, to, distance in km}
// ------------------------------------------------------------
// Distances marked (*) are ESTIMATED - Chakrata Road,
// Sahastradhara, and Forest Research Area were not on the
// team's map poster. Replace with real values if available.
struct Edge {
    int u, v;
    double distance;
};

vector<Edge> edgeList = {
    {0, 1, 2.6},   {0, 2, 2.4},   {0, 4, 2.1},   {0, 5, 3.0},
    {0, 9, 4.8},   {0, 11, 3.4},  {1, 8, 5.2},   {2, 3, 2.8},
    {2, 10, 3.1},  {3, 8, 4.5},   {4, 6, 3.2},   {4, 7, 2.3},
    {5, 7, 4.2},   {5, 13, 2.7},  {5, 14, 3.5},  {9, 12, 1.5},
    {11, 9, 4.0},  {11, 12, 3.0}, {13, 14, 1.2}
};

vector<vector<pair<int, double>>> adj;

void createGraph() {
    adj.assign(V, {});
    for (auto &e : edgeList) {
        adj[e.u].push_back({e.v, e.distance});
        adj[e.v].push_back({e.u, e.distance});
    }
}

// ------------------------------------------------------------
// 3. ANSI COLOUR SUPPORT
// ------------------------------------------------------------
const string GREEN = "\033[1;32m";
const string CYAN  = "\033[1;36m";
const string YELLOW= "\033[1;33m";
const string GREY  = "\033[0;37m";
const string RESET = "\033[0m";

void enableColors() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

// ------------------------------------------------------------
// 4. DISPLAY LOCATIONS (numbered list)
// ------------------------------------------------------------
void displayLocations() {
    cout << "\n----- CITY LOCATIONS -----\n";
    for (int i = 0; i < V; i++) {
        cout << i << ". " << locations[i].name;
        if (locations[i].isChargingStation)
            cout << "  [" << locations[i].stationId << " - Charging Station]";
        cout << "\n";
    }
    cout << "---------------------------\n";
}

void printNodeLabel(int node) {
    cout << "[" << node << "] " << locations[node].name;
    if (locations[node].isChargingStation)
        cout << " (" << locations[node].stationId << ")";
}

// ------------------------------------------------------------
// 5. MAP-STYLE CANVAS DRAWING
// ------------------------------------------------------------
const int ROWS = 38;
const int COLS = 118;

// cellType: 0 = empty, 1 = road line / distance text, 2 = node label
enum CellType { EMPTY = 0, ROAD = 1, LABEL = 2, DIST = 3 };

bool isEdgeInPath(int a, int b, const vector<int> &path) {
    for (size_t i = 0; i + 1 < path.size(); i++) {
        if ((path[i] == a && path[i + 1] == b) ||
            (path[i] == b && path[i + 1] == a))
            return true;
    }
    return false;
}

void placeNodeLabels(vector<string> &canvas, vector<int> &cellType,
                      vector<bool> &highlightMask, const vector<int> &highlightPath) {
    for (int id = 0; id < V; id++) {
        string text = "[" + to_string(id) + "]" + locations[id].shortLabel;
        if (locations[id].isChargingStation)
            text += "(" + locations[id].stationId + ")";

        int r = locations[id].row;
        int c = locations[id].col;
        bool onPath = find(highlightPath.begin(), highlightPath.end(), id) != highlightPath.end();

        for (size_t k = 0; k < text.size(); k++) {
            int cc = c + (int)k;
            if (r < 0 || r >= ROWS || cc < 0 || cc >= COLS) continue;
            canvas[r][cc] = text[k];
            cellType[r * COLS + cc] = LABEL;
            highlightMask[r * COLS + cc] = onPath;
        }
    }
}