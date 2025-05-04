#include "Stronghold.h"

// ======== Leader Base Class ========

Leader::Leader(string n) {
    name = n;
}

string Leader::getName() const {
    return name;
}

// Default imposePolicy (does nothing)
void Leader::imposePolicy(Economy& economy, Army& army) {
    cout << name << " has not imposed any specific policy.\n";
}

// ======== King Subclass ========

King::King() : Leader("King") {}

void King::imposePolicy_King(Economy& economy, Army& army) {
    cout << "The King enacts a peace policy.\n";
    economy.spend(100); // Invest in public happiness
    army.lowerMorale(5); // Soldiers bored without war
}

// ======== Tyrant Subclass ========

Tyrant::Tyrant() : Leader("Tyrant") {}

void Tyrant::imposePolicy_Tyrant(Economy& economy, Army& army) {
    cout << "The Tyrant enforces harsh military rules.\n";
    economy.spend(50);       // Spend less
    army.lowerMorale(-10);   // Increase morale (aggressive spirit)
}
