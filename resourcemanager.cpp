#include "Stronghold.h"

// Constructor
ResourceManager::ResourceManager() {
    food = 500;
    wood = 300;
    stone = 200;
    iron = 100;
}

// Helper method to track resource changes
void ResourceManager::trackResourceChange(int& resource, int change, const std::string& resourceType, const std::string& action) {
    int oldValue = resource;
    resource += change;
    
    // This can be used for logging with GameSaver later
    // Example: gameSaver.logResourceChange(resourceType, oldValue, resource, action);
}

// General resource management simulation
void ResourceManager::manage() {
    cout << "\n--- Resource Management ---\n";
    cout << "Choose operation:\n";
    cout << "1. Gather Resources\n";
    cout << "2. Consume Resources\n";
    int choice;
    cin >> choice;

    if (choice == 1) {
        gatherResources();
    } else if (choice == 2) {
        consumeResources();
    } else {
        cout << "Invalid option.\n";
    }
}

// Gather resources (user inputs how much to add)
void ResourceManager::gatherResources() {
    int f, w, s, i;

    cout << "\nEnter amount of each resource to gather:\n";
    cout << "Food: "; cin >> f;
    cout << "Wood: "; cin >> w;
    cout << "Stone: "; cin >> s;
    cout << "Iron: "; cin >> i;

    if (f < 0 || w < 0 || s < 0 || i < 0) {
        cout << "Invalid input. Cannot gather negative resources.\n";
        return;
    }

    // Use the helper method to track resource changes
    trackResourceChange(food, f, "FOOD", "Gathering");
    trackResourceChange(wood, w, "WOOD", "Gathering");
    trackResourceChange(stone, s, "STONE", "Gathering");
    trackResourceChange(iron, i, "IRON", "Gathering");

    cout << "Resources gathered successfully.\n";
}

// Consume resources (user inputs how much to use)
void ResourceManager::consumeResources() {
    int f, w, s, i;

    cout << "\nEnter amount of each resource to consume:\n";
    cout << "Food: "; cin >> f;
    cout << "Wood: "; cin >> w;
    cout << "Stone: "; cin >> s;
    cout << "Iron: "; cin >> i;

    if (f < 0 || w < 0 || s < 0 || i < 0) {
        cout << "Invalid input. Cannot consume negative resources.\n";
        return;
    }

    if (food < f || wood < w || stone < s || iron < i) {
        cout << "Insufficient resources. Consumption failed.\n";
        return;
    }

    // Use the helper method to track resource changes
    trackResourceChange(food, -f, "FOOD", "Consumption");
    trackResourceChange(wood, -w, "WOOD", "Consumption");
    trackResourceChange(stone, -s, "STONE", "Consumption");
    trackResourceChange(iron, -i, "IRON", "Consumption");

    cout << "Resources consumed successfully.\n";
}

// Show current stock
void ResourceManager::showStats() const {
    cout << "\n====== Resource Stock ======\n";
    cout << "Food: " << food << "\n";
    cout << "Wood: " << wood << "\n";
    cout << "Stone: " << stone << "\n";
    cout << "Iron: " << iron << "\n";
}

// Save to file
void ResourceManager::saveToFile() const {
    ofstream out("resources.txt");
    if (!out) {
        cout << "Error: Could not save resources.\n";
        return;
    }

    out << food << endl;
    out << wood << endl;
    out << stone << endl;
    out << iron << endl;
    out.close();
    cout << "Resources saved to file.\n";
}

// Load from file
void ResourceManager::loadFromFile() {
    ifstream in("resources.txt");
    if (!in) {
        cout << "Error: Could not load resources.\n";
        return;
    }

    in >> food >> wood >> stone >> iron;
    in.close();
    cout << "Resources loaded from file.\n";
}
void ResourceManager::consumeFixed(string resourceType, int amount) {
    if (resourceType == "food" && food >= amount) {
        trackResourceChange(food, -amount, "FOOD", "Fixed consumption");
    }
    else if (resourceType == "wood" && wood >= amount) {
        trackResourceChange(wood, -amount, "WOOD", "Fixed consumption");
    }
    else if (resourceType == "stone" && stone >= amount) {
        trackResourceChange(stone, -amount, "STONE", "Fixed consumption");
    }
    else if (resourceType == "iron" && iron >= amount) {
        trackResourceChange(iron, -amount, "IRON", "Fixed consumption");
    }
    else {
        cout << "Not enough " << resourceType << " available.\n";
    }
}
