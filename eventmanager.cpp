#include "Stronghold.h"

// Constructor
EventManager::EventManager() {
    // No initialization needed
}

// Trigger an event manually chosen by the user
void EventManager::trigger(Population& pop, Army& army, Economy& eco, ResourceManager& res) {
    cout << "\nEvent Trigger Menu\n";
    cout << "Choose an event to trigger:\n";
    cout << "1. Famine\n";
    cout << "2. Disease Outbreak\n";
    cout << "3. War Attack\n";
    cout << "4. Betrayal by Nobles\n";
    cout << "5. Earthquake (Stone Loss)\n";
    int choice;
    cin >> choice;

    if (choice == 1) {
        famine(res, pop);
    } else if (choice == 2) {
        disease(pop);
    } else if (choice == 3) {
        war(army, eco);
    } else if (choice == 4) {
        betrayal(eco);
    } else if (choice == 5) {
        earthquake(res);
    } else {
        cout << "Invalid choice.\n";
    }
}

// ========== Event Implementations ==========

void EventManager::famine(ResourceManager& res, Population& pop) {
    cout << "Famine hits the land! Food reduced by 100.\n";
    res.consumeFixed("food", 100);
    pop.decrease(10);
}

void EventManager::disease(Population& pop) {
    cout << "A deadly disease spreads! 15 people lost.\n";
    pop.decrease(15);
}

void EventManager::war(Army& army, Economy& eco) {
    cout << "War erupts! Army loses morale and treasury suffers.\n";
    army.lowerMorale(20);
    eco.spend(200);
}

void EventManager::betrayal(Economy& eco) {
    cout << "Noble betrayal! 300 gold stolen from treasury.\n";
    eco.spend(300);
}

void EventManager::earthquake(ResourceManager& res) {
    cout << "Earthquake shakes the kingdom! Stone supply drops.\n";
    res.consumeFixed("stone", 50);
}
