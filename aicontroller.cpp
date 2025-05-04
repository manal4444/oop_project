#include "Stronghold.h"

// Constructor initializes AI state variables
AIController::AIController() {
    // Initialize with moderate risk tolerance
    riskTolerance = 0.5f;
    lastTaxCollection = 0;
    lastArmySize = 0;
    conflictLevel = 3; // Start with moderate conflict level
    
    // Initialize dynamic arrays using new keyword
    decisionHistoryCapacity = 10;
    decisionHistorySize = 0;
    decisionHistory = new int[decisionHistoryCapacity];
    
    // Initialize unit strength factors
    unitTypesCount = 5; // Infantry, Cavalry, Archers, Siege, Special
    unitStrengthFactors = new float[unitTypesCount];
    for (int i = 0; i < unitTypesCount; i++) {
        unitStrengthFactors[i] = 1.0f; // Default strength factor
    }
    
    // Initialize resource allocation priorities
    resourceTypesCount = 4; // Gold, Food, Wood, Stone
    resourceAllocation = new int[resourceTypesCount];
    for (int i = 0; i < resourceTypesCount; i++) {
        resourceAllocation[i] = 25; // Equal priority by default (25% each)
    }
}

// Destructor to clean up dynamically allocated memory
AIController::~AIController() {
    // Free all dynamically allocated arrays using delete[] operator
    delete[] decisionHistory;
    delete[] unitStrengthFactors;
    delete[] resourceAllocation;
    
    // Set pointers to nullptr to avoid dangling pointers
    decisionHistory = nullptr;
    unitStrengthFactors = nullptr;
    resourceAllocation = nullptr;
}

// Helper method to calculate appropriate tax rate based on economic and population factors
float AIController::calculateTaxRate(const Economy& eco, const Population& pop) const {
    // Base tax rate calculation
    float baseRate = 0.1f; // 10% base tax rate
    
    // Adjust based on population size (larger populations can handle lower rates)
    float popFactor = pop.getTotal() > 1000 ? 0.02f : 0.05f;
    
    // Adjust based on risk tolerance (higher risk = higher taxes)
    float riskFactor = riskTolerance * 0.1f;
    
    return baseRate + popFactor + riskFactor;
}

// Helper method to determine how many troops to recruit
int AIController::determineRecruitmentNeeds(const Army& army, const Population& pop) const {
    // Base recruitment is 5% of population
    int baseRecruitment = (int)(pop.getTotal() * 0.05f);
    
    // Adjust based on current army size (smaller armies need more recruits)
    int currentSoldiers = army.getSoldiers();
    float armySizeFactor = currentSoldiers < 100 ? 1.5f : 1.0f;
    
    // Adjust based on risk tolerance (higher risk = larger army)
    float riskFactor = 1.0f + riskTolerance;
    
    return (int)(baseRecruitment * armySizeFactor * riskFactor);
}

// Helper method to assess conflict severity
int AIController::assessConflictSeverity(const Population& pop, const Economy& eco) const {
    // Base conflict level (0-10 scale)
    int baseConflict = conflictLevel;
    
    // Economic factors affect conflict (poor economy = more conflict)
    int treasuryFactor = eco.getTreasury() < 500 ? 2 : 0;
    
    // Population factors (larger populations are harder to control)
    int popFactor = pop.getTotal() > 1000 ? 1 : 0;
    
    // Calculate total severity (capped at 10)
    int severity = baseConflict + treasuryFactor + popFactor;
    return severity > 10 ? 10 : severity;
}

// Method to add a decision to the history array
void AIController::addDecision(int decisionCode) {
    // Check if we need to resize the array
    if (decisionHistorySize >= decisionHistoryCapacity) {
        // Create a new larger array
        int newCapacity = decisionHistoryCapacity * 2;
        int* newHistory = new int[newCapacity];
        
        // Copy existing elements to the new array
        for (int i = 0; i < decisionHistorySize; i++) {
            newHistory[i] = decisionHistory[i];
        }
        
        // Delete the old array and update pointers
        delete[] decisionHistory;
        decisionHistory = newHistory;
        decisionHistoryCapacity = newCapacity;
    }
    
    // Add the new decision to the array
    decisionHistory[decisionHistorySize++] = decisionCode;
}

// Method to update the strength factor for a specific unit type
void AIController::updateUnitStrength(int unitType, float newStrength) {
    // Validate unit type index
    if (unitType >= 0 && unitType < unitTypesCount) {
        // Ensure strength stays within reasonable bounds (0.5 to 2.0)
        if (newStrength < 0.5f) newStrength = 0.5f;
        if (newStrength > 2.0f) newStrength = 2.0f;
        
        unitStrengthFactors[unitType] = newStrength;
    }
}

// Method to set the priority for a specific resource type
void AIController::setResourcePriority(int resourceType, int priority) {
    // Validate resource type index
    if (resourceType >= 0 && resourceType < resourceTypesCount) {
        // Ensure priority is within valid range (0-100)
        if (priority < 0) priority = 0;
        if (priority > 100) priority = 100;
        
        // Update the priority for this resource
        resourceAllocation[resourceType] = priority;
        
        // Normalize all priorities to ensure they sum to 100
        int totalPriority = 0;
        for (int i = 0; i < resourceTypesCount; i++) {
            totalPriority += resourceAllocation[i];
        }
        
        // If total is not 100, adjust all priorities proportionally
        if (totalPriority != 100 && totalPriority > 0) {
            for (int i = 0; i < resourceTypesCount; i++) {
                resourceAllocation[i] = (resourceAllocation[i] * 100) / totalPriority;
            }
        }
    }
}

// Main decision method for taxation
string AIController::makeTaxDecision(Economy& eco, Population& pop) {
    string report = "\n[AI TAX DECISION]\n";
    report += "Analyzing kingdom economic state...\n";
    
    // Calculate optimal tax rate
    float taxRate = calculateTaxRate(eco, pop);
    int treasuryBefore = eco.getTreasury();
    
    // Create a temporary array for tax rate analysis by population segment
    float* taxRateBySegment = new float[3]; // 3 segments: peasants, merchants, nobles
    taxRateBySegment[0] = taxRate * 0.8f;  // Lower rate for peasants
    taxRateBySegment[1] = taxRate * 1.0f;  // Standard rate for merchants
    taxRateBySegment[2] = taxRate * 1.2f;  // Higher rate for nobles
    
    // Display tax rates by segment
    report += "Tax rate analysis by population segment:\n";
    report += "  Peasants: " + to_string((int)(taxRateBySegment[0] * 100)) + "%\n";
    report += "  Merchants: " + to_string((int)(taxRateBySegment[1] * 100)) + "%\n";
    report += "  Nobles: " + to_string((int)(taxRateBySegment[2] * 100)) + "%\n";
    
    // Apply the tax decision
    report += "AI Decision: Setting tax rate to " + to_string((int)(taxRate * 100)) + "%\n";
    report += "Reasoning: Based on population size and economic indicators\n";
    
    // Adjust resource allocation based on expected tax income
    report += "Adjusting resource allocation priorities:\n";
    for (int i = 0; i < resourceTypesCount; i++) {
        report += "  Resource " + to_string(i) + ": " + to_string(resourceAllocation[i]) + "% priority\n";
    }
    
    // Simulate tax collection
    eco.taxPopulation(pop);
    int treasuryAfter = eco.getTreasury();
    lastTaxCollection = treasuryAfter - treasuryBefore;
    
    report += "Tax collection complete. Treasury increased by " + to_string(lastTaxCollection) + " gold.\n";
    
    // Assess impact and provide feedback
    int decisionCode = 0;
    if (lastTaxCollection > 500) {
        report += "Result: Excellent tax revenue generated!\n";
        decisionCode = 20; // Code for excellent tax collection
    } else if (lastTaxCollection > 200) {
        report += "Result: Satisfactory tax revenue.\n";
        decisionCode = 21; // Code for satisfactory tax collection
    } else {
        report += "Result: Poor tax revenue. Will adjust strategy next time.\n";
        // Adjust risk tolerance based on results
        riskTolerance += 0.1f;
        if (riskTolerance > 1.0f) riskTolerance = 1.0f;
        decisionCode = 22; // Code for poor tax collection
    }
    
    // Record this decision in our history
    addDecision(decisionCode);
    
    // Clean up dynamically allocated memory
    delete[] taxRateBySegment;
    
    return report;
}

// Main decision method for army management
string AIController::mobilizeArmy(Army& army, Population& pop, ResourceManager& res) {
    string report = "\n[AI ARMY DECISION]\n";
    report += "Analyzing military needs and resources...\n";
    
    // Store current army size
    int armySizeBefore = army.getSoldiers();
    lastArmySize = armySizeBefore;
    
    // Calculate recruitment needs using unit strength factors
    int recruitmentTarget = determineRecruitmentNeeds(army, pop);
    
    // Apply unit strength factors to determine unit type distribution
    report += "Unit strength analysis:\n";
    for (int i = 0; i < unitTypesCount; i++) {
        report += "  Unit Type " + to_string(i) + ": Strength factor " + to_string(unitStrengthFactors[i]) + "\n";
    }
    
    // Adjust resource allocation based on army needs
    int* tempAllocation = new int[resourceTypesCount];
    for (int i = 0; i < resourceTypesCount; i++) {
        // Temporarily increase military resource allocation
        tempAllocation[i] = resourceAllocation[i];
        if (i == 0) { // Assuming index 0 is for military resources
            tempAllocation[i] += 10; // Increase military priority
        } else {
            tempAllocation[i] = max(0, tempAllocation[i] - 3); // Decrease other priorities
        }
    }
    
    report += "AI Decision: Recruiting " + to_string(recruitmentTarget) + " new soldiers\n";
    report += "Reasoning: Based on current threats and available population\n";
    
    // Execute recruitment
    army.recruitAndTrain(pop);
    int armySizeAfter = army.getSoldiers();
    int actualRecruitment = armySizeAfter - armySizeBefore;
    
    report += "Recruitment complete. Army increased by " + to_string(actualRecruitment) + " soldiers.\n";
    
    // Assess results and update unit strength factors based on performance
    if (actualRecruitment >= recruitmentTarget) {
        report += "Result: Recruitment goals met or exceeded.\n";
        // Increase strength factor for successful unit types
        updateUnitStrength(0, unitStrengthFactors[0] + 0.1f); // Improve infantry
    } else if (actualRecruitment >= recruitmentTarget / 2) {
        report += "Result: Partial recruitment success.\n";
    } else {
        report += "Result: Failed to meet recruitment goals. Will adjust strategy.\n";
        // Adjust risk tolerance based on results
        riskTolerance -= 0.1f;
        if (riskTolerance < 0.0f) riskTolerance = 0.0f;
        
        // Decrease strength factor for underperforming unit types
        updateUnitStrength(0, unitStrengthFactors[0] - 0.05f); // Reduce infantry reliance
    }
    
    // Clean up temporary allocation array
    delete[] tempAllocation;
    
    // Record decision
    addDecision(actualRecruitment >= recruitmentTarget ? 10 : 11);
    
    return report;
}

// Main decision method for handling internal conflicts
string AIController::handleInternalConflict(Population& pop, Army& army, Economy& eco) {
    string report = "\n[AI CONFLICT MANAGEMENT]\n";
    report += "Assessing internal kingdom stability...\n";
    
    // Assess conflict severity
    int severity = assessConflictSeverity(pop, eco);
    
    report += "Detected conflict level: " + to_string(severity) + "/10\n";
    
    // Make decisions based on severity
    int decisionCode = 0;
    if (severity > 7) {
        // High severity - use military force
        report += "AI Decision: Deploying military to suppress unrest\n";
        report += "Reasoning: High conflict level requires immediate action\n";
        
        // Simulate military action
        army.lowerMorale(2); // Military action affects morale
        conflictLevel -= 3;  // Reduce conflict level
        decisionCode = 3; // Code for military action
        
        report += "Military action taken. Conflict reduced but at cost to army morale.\n";
    } 
    else if (severity > 4) {
        // Medium severity - economic solution
        report += "AI Decision: Distributing funds to appease population\n";
        report += "Reasoning: Moderate conflict can be resolved with economic incentives\n";
        
        // Simulate economic action
        int appeasementCost = 100 + (severity * 20);
        eco.spend(appeasementCost);
        conflictLevel -= 2;
        decisionCode = 2; // Code for economic action
        
        report += "Spent " + to_string(appeasementCost) + " gold on public works and relief.\n";
        report += "Conflict reduced through economic means.\n";
    }
    else {
        // Low severity - monitor only
        report += "AI Decision: Monitoring situation, no action needed\n";
        report += "Reasoning: Conflict level is manageable\n";
        
        // Slight natural reduction in conflict
        conflictLevel -= 1;
        if (conflictLevel < 0) conflictLevel = 0;
        decisionCode = 1; // Code for monitoring action
        
        report += "Situation stable. Continuing to monitor.\n";
    }
    
    // Record this decision in our history
    addDecision(decisionCode);
    
    // Ensure conflict level stays in bounds
    if (conflictLevel < 0) conflictLevel = 0;
    if (conflictLevel > 10) conflictLevel = 10;
    
    report += "Current conflict level after actions: " + to_string(conflictLevel) + "/10\n";
    
    return report;
}