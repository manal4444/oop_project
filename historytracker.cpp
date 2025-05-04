#include "Stronghold.h"
#include<iostream>
#include<iomanip>
// Constructor initializes the dynamic array and counters
HistoryTracker::HistoryTracker() {
    capacity = 10;  // Initial capacity
    size = 0;
    currentTurn = 1; // Start at turn 1
    
    // Allocate memory for the snapshots array
    snapshots = new GameStateSnapshot[capacity];
}

// Destructor frees the dynamically allocated memory
HistoryTracker::~HistoryTracker() {
    // Free the snapshots array
    delete[] snapshots;
    snapshots = nullptr;
}

// Helper method to resize the snapshots array when it's full
void HistoryTracker::resizeSnapshotsArray() {
    // Double the capacity
    int newCapacity = capacity * 2;
    
    // Allocate a new array with the increased capacity
    GameStateSnapshot* newSnapshots = new GameStateSnapshot[newCapacity];
    if (!newSnapshots) {
        std::cerr << "[ERROR] Memory allocation failed during resizeSnapshotsArray. Exiting.\n";
        exit(1);
    }
    
    // Copy existing snapshots to the new array
    for (int i = 0; i < size; i++) {
        newSnapshots[i] = snapshots[i];
    }
    
    // Delete the old array
    delete[] snapshots;
    
    // Update the pointer and capacity
    snapshots = newSnapshots;
    capacity = newCapacity;
}

// Take a snapshot of the current game state
void HistoryTracker::takeSnapshot(const Population& pop, const Economy& eco, 
                                const Army& army, const ResourceManager& res,
                                const string& eventDescription) {
    // Check if we need to resize the array
    if (size >= capacity) {
        resizeSnapshotsArray();
    }
    
    // Create a new snapshot with current game state
    GameStateSnapshot snapshot;
    snapshot.turn = currentTurn;
    snapshot.population = pop.getTotal();
    snapshot.treasury = eco.getTreasury();
    snapshot.soldiers = army.getSoldiers();
    snapshot.morale = army.getMorale();
    snapshot.food = res.getFood();
    snapshot.wood = res.getWood();
    snapshot.stone = res.getStone();
    snapshot.iron = res.getIron();
    snapshot.eventDescription = eventDescription;
    
    // Add the snapshot to the array
    snapshots[size++] = snapshot;
    
    cout << "\n[HISTORY] Snapshot taken at turn " << currentTurn << "\n";
}

// Increment the turn counter
void HistoryTracker::nextTurn() {
    currentTurn++;
    cout << "\n[HISTORY] Advanced to turn " << currentTurn << "\n";
}

// Display the history as a progression report
void HistoryTracker::displayProgressionReport() const {
    cout << "\n===============================================\n";
    cout << "           KINGDOM HISTORY REPORT           \n";
    cout << "===============================================\n";
    
    if (size == 0) {
        cout << "No historical data available.\n";
        return;
    }
    
    // Display header
    cout << "Turn | Population | Treasury | Soldiers | Morale | Food | Wood | Stone | Iron | Event\n";
    cout << "-----|------------|----------|----------|--------|------|------|-------|------|-------\n";
    
    // Display each snapshot
    for (int i = 0; i < size; i++) {
        if (i < 0 || i >= capacity) {
            std::cerr << "[ERROR] Out-of-bounds access in displayProgressionReport.\n";
            break;
        }
        GameStateSnapshot& snap = snapshots[i];
        
        // Format the output with fixed width columns
        cout << setw(4) << snap.turn << " | ";
        cout << setw(10) << snap.population << " | ";
        cout << setw(8) << snap.treasury << " | ";
        cout << setw(8) << snap.soldiers << " | ";
        cout << setw(6) << snap.morale << " | ";
        cout << setw(4) << snap.food << " | ";
        cout << setw(4) << snap.wood << " | ";
        cout << setw(5) << snap.stone << " | ";
        cout << setw(4) << snap.iron << " | ";
        
        // Truncate event description if too long
        string event = snap.eventDescription;
        if (event.length() > 30) {
            event = event.substr(0, 27) + "...";
        }
        cout << event << "\n";
    }
    
    // Display summary of changes
    if (size > 1) {
        cout << "\n===============================================\n";
        cout << "              CHANGE SUMMARY               \n";
        cout << "===============================================\n";
        
        GameStateSnapshot& first = snapshots[0];
        GameStateSnapshot& last = snapshots[size-1];
        
        int popChange = last.population - first.population;
        int treasuryChange = last.treasury - first.treasury;
        int soldiersChange = last.soldiers - first.soldiers;
        int moraleChange = last.morale - first.morale;
        int foodChange = last.food - first.food;
        int woodChange = last.wood - first.wood;
        int stoneChange = last.stone - first.stone;
        int ironChange = last.iron - first.iron;
        
        cout << "Population: " << first.population << " -> " << last.population;
        cout << " (" << (popChange >= 0 ? "+" : "") << popChange << ")\n";
        
        cout << "Treasury: " << first.treasury << " -> " << last.treasury;
        cout << " (" << (treasuryChange >= 0 ? "+" : "") << treasuryChange << ")\n";
        
        cout << "Soldiers: " << first.soldiers << " -> " << last.soldiers;
        cout << " (" << (soldiersChange >= 0 ? "+" : "") << soldiersChange << ")\n";
        
        cout << "Morale: " << first.morale << " -> " << last.morale;
        cout << " (" << (moraleChange >= 0 ? "+" : "") << moraleChange << ")\n";
        
        cout << "Food: " << first.food << " -> " << last.food;
        cout << " (" << (foodChange >= 0 ? "+" : "") << foodChange << ")\n";
        
        cout << "Wood: " << first.wood << " -> " << last.wood;
        cout << " (" << (woodChange >= 0 ? "+" : "") << woodChange << ")\n";
        
        cout << "Stone: " << first.stone << " -> " << last.stone;
        cout << " (" << (stoneChange >= 0 ? "+" : "") << stoneChange << ")\n";
        
        cout << "Iron: " << first.iron << " -> " << last.iron;
        cout << " (" << (ironChange >= 0 ? "+" : "") << ironChange << ")\n";
    }
    
    cout << "\nEnd of Kingdom History Report\n";
    cout << "===============================================\n";
}

// Get the current turn
int HistoryTracker::getCurrentTurn() const {
    return currentTurn;
}