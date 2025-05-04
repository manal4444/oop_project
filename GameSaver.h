#pragma once
#include "Stronghold.h"
#include <fstream>
#include <string>
#include <ctime>

class GameSaver {
private:
    std::string gameStatePath;
    std::string scoreLogPath;
    
    // Helper method to get current timestamp
    std::string getTimestamp() const {
        time_t now = time(0);
        char buffer[80];
        struct tm timeinfo;
        localtime_s(&timeinfo, &now);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
        return std::string(buffer);
    }
    
public:
    GameSaver(const std::string& gameFile = "game_save.txt", const std::string& scoreFile = "score.txt")
        : gameStatePath(gameFile), scoreLogPath(scoreFile) {}
    
    // Save all game state to a single file
    bool saveGame(const Population& pop, const Army& army, const Economy& eco, 
                 const ResourceManager& res, const Bank& bank) const;
    
    // Load all game state from a single file
    bool loadGame(Population& pop, Army& army, Economy& eco, 
                 ResourceManager& res, Bank& bank) const;
    
    // Log resource changes with timestamp
    void logResourceChange(const std::string& resourceType, int oldValue, int newValue, 
                          const std::string& action) const;
    
    // Log score and event with timestamp
    void logEvent(const std::string& eventType, const std::string& description) const;
};