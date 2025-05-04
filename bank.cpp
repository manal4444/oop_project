#include "Stronghold.h"

// Constructor
Bank::Bank() {
    loansIssued = 0;
    fraudDetected = 0;
}

// Audit treasury for possible fraud (basic check)
void Bank::auditTreasury(Economy& economy) {
    cout << "\nBank Audit in Progress...\n";

    int treasury = economy.getTreasury();

    if (treasury < 0) {
        cout << "Fraud Detected! Treasury has negative balance.\n";
        fraudDetected++;
    } else if (treasury < 100) {
        cout << "Low Treasury Warning: Only " << treasury << " gold left.\n";
    } else {
        cout << "Treasury audit passed. Gold is safe.\n";
    }
}

// Issue a loan (adds to treasury, increases debt)
void Bank::issueLoan(Economy& economy, int amount) {
    cout << "\nIssuing loan of " << amount << " gold...\n";

    if (amount <= 0) {
        cout << "Invalid loan amount.\n";
        return;
    }

    economy.receiveLoan(amount); // increase treasury
    loansIssued += amount;
    cout << "Loan added to treasury. Total loans issued: " << loansIssued << "\n";
}

// Repay a loan (reduces treasury)
void Bank::repayLoan(Economy& economy, int amount) {
    cout << "\nRepaying loan of " << amount << " gold...\n";

    if (amount <= 0 || amount > loansIssued) {
        cout << "Invalid repayment amount.\n";
        return;
    }

    int currentTreasury = economy.getTreasury();
    if (amount > currentTreasury) {
        cout << "Not enough gold to repay loan.\n";
        return;
    }

    economy.spend(amount);
    loansIssued -= amount;
    cout << "Loan repaid. Remaining debt: " << loansIssued << "\n";
}

// Show current banking info
void Bank::showStats() const {
    cout << "\n====== Bank Summary ======\n";
    cout << "Total Loans Issued: " << loansIssued << " gold\n";
    cout << "Fraud Cases Detected: " << fraudDetected << "\n";
}

// Save to file
void Bank::saveToFile() const {
    ofstream out("bank.txt");
    if (!out) {
        cout << "Error: Could not save bank data.\n";
        return;
    }

    out << loansIssued << endl;
    out << fraudDetected << endl;
    out.close();

    cout << "Bank data saved successfully.\n";
}

// Load from file
void Bank::loadFromFile() {
    ifstream in("bank.txt");
    if (!in) {
        cout << "Error: Could not load bank data.\n";
        return;
    }

    in >> loansIssued >> fraudDetected;
    in.close();

    cout << "Bank data loaded successfully.\n";
}
