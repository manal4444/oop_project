#include "Stronghold.h"

// Constructor
Army::Army() {
    soldiers = 20;
    morale = 70;       // 0–100 scale
    foodSupply = 100;  // units of food for the army
}

// Recruit and train soldiers from population
void Army::recruitAndTrain(Population& pop) {
    cout << "\n--- Army Recruitment & Training ---\n";

    int recruitCount;
    cout << "Enter number of soldiers to recruit: ";
    cin >> recruitCount;

    if (recruitCount <= 0 || recruitCount > pop.getTotal()) {
        cout << "Invalid number of recruits. Aborting...\n";
        return;
    }

    int foodRequired = recruitCount * 2;
    if (foodSupply < foodRequired) {
        cout << "Not enough food to train " << recruitCount << " soldiers!\n";
        morale -= 10;
        return;
    }

    pop.decrease(recruitCount); // Decrease population
    
    // Use the helper method to track resource changes
    trackResourceChange(soldiers, recruitCount, "SOLDIERS", "Recruitment");
    trackResourceChange(foodSupply, -foodRequired, "FOOD_SUPPLY", "Army training");
    trackResourceChange(morale, 5, "MORALE", "Recruitment boost");

    // Clamp values
    if (morale > 100) morale = 100;
    if (morale < 0) morale = 0;

    cout << recruitCount << " soldiers recruited and trained.\n";
    cout << "Food used: " << foodRequired << "\n";
    cout << "Current morale: " << morale << "%\n";
}

// Display current army stats
void Army::showStats() const {
    cout << "\n====== Army Stats ======\n";
    cout << "Total Soldiers: " << soldiers << endl;
    cout << "Morale: " << morale << "%\n";
    cout << "Food Supply: " << foodSupply << " units\n";
}

// Save to file
void Army::saveToFile() const {
    ofstream out("army.txt");
    if (!out) {
        cerr << "Error: Could not save army data.\n";
        return;
    }

    out << soldiers << endl;
    out << morale << endl;
    out << foodSupply << endl;
    out.close();
    cout << "Army data saved successfully.\n";
}

// Load from file
void Army::loadFromFile() {
    ifstream in("army.txt");
    if (!in) {
        cerr << "Error: Could not load army data.\n";
        return;
    }

    in >> soldiers >> morale >> foodSupply;
    in.close();
    cout << "Army data loaded successfully.\n";
}

// Lower morale (used by events, economic trouble etc.)
void Army::lowerMorale(int amount) {
    int oldMorale = morale;
    morale -= amount;
    if (morale < 0) morale = 0;
    
    // This can be used for logging with GameSaver later
    // Example: gameSaver.logResourceChange("MORALE", oldMorale, morale, "Event impact");
}

// Helper method to track resource changes
void Army::trackResourceChange(int& resource, int change, const std::string& resourceType, const std::string& action) {
    int oldValue = resource;
    resource += change;
    
    // This can be used for logging with GameSaver later
    // Example: gameSaver.logResourceChange(resourceType, oldValue, resource, action);
}
