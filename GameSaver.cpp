#include "Stronghold.h"
// Save all game state to a single file
bool GameSaver::saveGame(const Population& pop, const Army& army, const Economy& eco, 
                       const ResourceManager& res, const Bank& bank) const {
    std::ofstream out(gameStatePath);
    if (!out) {
        std::cerr << "Error: Could not open " << gameStatePath << " for saving game state.\n";
        return false;
    }
    
    // Write a header with timestamp
    out << "# Stronghold Game Save - " << getTimestamp() << std::endl;
    
    // Write Population data with section header
    out << "[POPULATION]" << std::endl;
    out << pop.getTotal() << std::endl;
    
    // Write Army data with section header
    out << "[ARMY]" << std::endl;
    
    // Write Economy data with section header
    out << "[ECONOMY]" << std::endl;
    out << eco.getTreasury() << std::endl;
    
    // Write Resource data with section header
    out << "[RESOURCES]" << std::endl;
    
    // Write Bank data with section header
    out << "[BANK]" << std::endl;
    
    out.close();
    
    // Log the save event
    logEvent("GAME_SAVE", "Game state saved to " + gameStatePath);
    
    std::cout << "Game saved successfully to " << gameStatePath << std::endl;
    return true;
}

// Load all game state from a single file
bool GameSaver::loadGame(Population& pop, Army& army, Economy& eco, 
                       ResourceManager& res, Bank& bank) const {
    std::ifstream in(gameStatePath);
    if (!in) {
        std::cerr << "Error: Could not open " << gameStatePath << " for loading game state.\n";
        return false;
    }
    
    std::string line;
    std::string currentSection = "";
    
    // Skip header line
    std::getline(in, line);
    
    while (std::getline(in, line)) {
        // Check if this is a section header
        if (line.length() > 2 && line[0] == '[' && line[line.length()-1] == ']') {
            currentSection = line;
            continue;
        }
        
        // Process data based on current section
        if (currentSection == "[POPULATION]") {
            // Load population data
            // This is simplified - in reality, we would need to load all population attributes
            // pop.loadFromFile();
        }
        else if (currentSection == "[ARMY]") {
            // Load army data
            // army.loadFromFile();
        }
        else if (currentSection == "[ECONOMY]") {
            // Load economy data
            // eco.loadFromFile();
        }
        else if (currentSection == "[RESOURCES]") {
            // Load resource data
            // res.loadFromFile();
        }
        else if (currentSection == "[BANK]") {
            // Load bank data
            // bank.loadFromFile();
        }
    }
    
    in.close();
    
    // Log the load event
    logEvent("GAME_LOAD", "Game state loaded from " + gameStatePath);
    
    std::cout << "Game loaded successfully from " << gameStatePath << std::endl;
    return true;
}

// Log resource changes with timestamp
void GameSaver::logResourceChange(const std::string& resourceType, int oldValue, int newValue, 
                                const std::string& action) const {
    std::ofstream log(scoreLogPath, std::ios::app); // Open in append mode
    if (!log) {
        std::cerr << "Error: Could not open " << scoreLogPath << " for logging.\n";
        return;
    }
    
    int change = newValue - oldValue;
    std::string changeStr = (change >= 0) ? "+" + std::to_string(change) : std::to_string(change);
    
    log << getTimestamp() << " | RESOURCE | " << resourceType << " | " 
        << oldValue << " -> " << newValue << " (" << changeStr << ") | " 
        << action << std::endl;
    
    log.close();
}

// Log score and event with timestamp
void GameSaver::logEvent(const std::string& eventType, const std::string& description) const {
    std::ofstream log(scoreLogPath, std::ios::app); // Open in append mode
    if (!log) {
        std::cerr << "Error: Could not open " << scoreLogPath << " for logging.\n";
        return;
    }
    
    log << getTimestamp() << " | EVENT | " << eventType << " | " << description << std::endl;
    
    log.close();
}