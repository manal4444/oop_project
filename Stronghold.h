#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

// ================== Forward Declarations ==================
class Army;
class Population;
class Economy;
class Bank;
class ResourceManager;
class EventManager;
class Leader;

// ================== Base Classes ==================

class Person {
protected:
    string name;
    int age;
public:
    Person();
    Person(string n, int a);
    virtual void display() const;
};

// ================== Social Structure ==================

class SocialClass {
protected:
    string title;
    int population;
public:
    SocialClass();
    SocialClass(string t, int pop);
    virtual void showStatus() const;
};

// ================== Population ==================

class Population {
private:
    int total;
    int peasants, merchants, nobles,foodStock;
    float happiness;
public:
    Population();
    void simulate();
    void showStats() const;
    void saveToFile() const;
    void loadFromFile();
    int getTotal() const;
    void decrease(int amount);
};

// ================== Army ==================

class Army {
private:
    int soldiers;
    int morale;
    int foodSupply;
    
    // Helper method to track resource changes
    void trackResourceChange(int& resource, int change, const  string& resourceType, const  string& action);
public:
    Army();
    void recruitAndTrain(Population& pop);
    void showStats() const;
    void saveToFile() const;
    void loadFromFile();
    void lowerMorale(int amount);
    
    // Getters for GameSaver
    int getSoldiers() const { return soldiers; }
    int getMorale() const { return morale; }
    int getFoodSupply() const { return foodSupply; }
};

// ================== Economy ==================

class Economy {
private:
    int treasury;
    float taxRate;
    float inflation;
public:
    Economy();
    void taxPopulation(const Population& pop);
    void spend(int amount);
    void showStats() const;
    void saveToFile() const;
    void loadFromFile();
    int getTreasury() const;
    void receiveLoan(int amount);
};

// ================== Bank ==================

class Bank {
    private:
        int loansIssued;
        int fraudDetected;
    public:
        Bank();
        void auditTreasury(Economy& economy);
        void issueLoan(Economy& economy, int amount);
        void repayLoan(Economy& economy, int amount);
        void showStats() const;
        void saveToFile() const;
        void loadFromFile();
    };
    

// ================== Resource Manager ==================

class ResourceManager {
    private:
        int food;
        int wood;
        int stone;
        int iron;
        
        // Helper method to track resource changes
        void trackResourceChange(int& resource, int change, const  string& resourceType, const  string& action);
    
    public:
        ResourceManager();
        void manage();
        void gatherResources();
        void consumeResources();
        void showStats() const;
        void saveToFile() const;
        void loadFromFile();
        void consumeFixed(string resourceType, int amount);
        
        // Getters for GameSaver
        int getFood() const { return food; }
        int getWood() const { return wood; }
        int getStone() const { return stone; }
        int getIron() const { return iron; }
    };
    
// ================== Event Manager ==================

class EventManager {
    public:
        EventManager();
        void trigger(Population& pop, Army& army, Economy& eco, ResourceManager& res);
        void famine(ResourceManager& res, Population& pop);
        void disease(Population& pop);
        void war(Army& army, Economy& eco);
        void betrayal(Economy& eco);
        void earthquake(ResourceManager& res);
    };
    
// ================== Leadership ==================

class Leader {
    protected:
        string name;
    public:
        Leader(string n);
        void imposePolicy(Economy& economy, Army& army); // Will dispatch manually
        string getName() const;
    };
    
    class King : public Leader {
    public:
        King();
        void imposePolicy_King(Economy& economy, Army& army);
    };
    
    class Tyrant : public Leader {
    public:
        Tyrant();
        void imposePolicy_Tyrant(Economy& economy, Army& army);
    };
    
// ================== Game Saver ==================

class GameSaver {
private:
     string gameStatePath;
     string scoreLogPath;
    
    // Helper method to get current timestamp
     string getTimestamp() const {
        time_t now = time(0);
        char buffer[80];
        struct tm timeinfo;
        localtime_s(&timeinfo, &now);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
        return  string(buffer);
    }
    
public:
    GameSaver(const  string& gameFile = "game_save.txt", const  string& scoreFile = "score.txt")
        : gameStatePath(gameFile), scoreLogPath(scoreFile) {}
    
    // Save all game state to a single file
    bool saveGame(const Population& pop, const Army& army, const Economy& eco, 
                 const ResourceManager& res, const Bank& bank) const {
        ofstream out(gameStatePath);
        if (!out) {
             cerr << "Error: Could not open " << gameStatePath << " for saving game state.\n";
            return false;
        }
        out << "# Stronghold Game Save - " << getTimestamp() <<  endl;
        out << "[POPULATION]" <<  endl;
        out << pop.getTotal() <<  endl;
        out << "[ARMY]" <<  endl;
        out << "[ECONOMY]" <<  endl;
        out << eco.getTreasury() <<  endl;
        out << "[RESOURCES]" <<  endl;
        out << "[BANK]" <<  endl;
        out.close();
        logEvent("GAME_SAVE", "Game state saved to " + gameStatePath);
         cout << "Game saved successfully to " << gameStatePath <<  endl;
        return true;
    }
    
    // Load all game state from a single file
    bool loadGame(Population& pop, Army& army, Economy& eco, 
                 ResourceManager& res, Bank& bank) const {
         ifstream in(gameStatePath);
        if (!in) {
             cerr << "Error: Could not open " << gameStatePath << " for loading game state.\n";
            return false;
        }
         string line;
         string currentSection = "";
         getline(in, line);
        while ( getline(in, line)) {
            if (line.length() > 2 && line[0] == '[' && line[line.length()-1] == ']') {
                currentSection = line;
                continue;
            }
            if (currentSection == "[POPULATION]") {
                // pop.loadFromFile();
            }
            else if (currentSection == "[ARMY]") {
                // army.loadFromFile();
            }
            else if (currentSection == "[ECONOMY]") {
                // eco.loadFromFile();
            }
            else if (currentSection == "[RESOURCES]") {
                // res.loadFromFile();
            }
            else if (currentSection == "[BANK]") {
                // bank.loadFromFile();
            }
        }
        in.close();
        logEvent("GAME_LOAD", "Game state loaded from " + gameStatePath);
         cout << "Game loaded successfully from " << gameStatePath <<  endl;
        return true;
    }
    
    // Log resource changes with timestamp
    void logResourceChange(const  string& resourceType, int oldValue, int newValue, 
                          const  string& action) const {
         ofstream out(scoreLogPath,  ios::app);
        if (out) {
            out << getTimestamp() << " [RESOURCE] " << resourceType << ": " << action << " from " << oldValue << " to " << newValue <<  endl;
            out.close();
        }
    }
    // Log score and event with timestamp
    void logEvent(const  string& eventType, const  string& description) const {
         ofstream out(scoreLogPath,  ios::app);
        if (out) {
            out << getTimestamp() << " [" << eventType << "] " << description <<  endl;
            out.close();
        }
    }
};

// ================== AI Controller ==================

class AIController {
private:
    float riskTolerance;    // 0.0 to 1.0, affects decision making
    int lastTaxCollection;  // Tracks last tax collection amount
    int lastArmySize;       // Tracks previous army size
    int conflictLevel;      // Tracks internal conflict level (0-10)
    
    // Dynamically allocated arrays
    int* decisionHistory;   // Array to track past decisions
    int decisionHistorySize;
    int decisionHistoryCapacity;
    
    float* unitStrengthFactors; // Array of strength multipliers for different unit types
    int unitTypesCount;
    
    int* resourceAllocation;    // Array to track resource allocation priorities
    int resourceTypesCount;
    
    // Helper methods for decision making
    float calculateTaxRate(const Economy& eco, const Population& pop) const;
    int determineRecruitmentNeeds(const Army& army, const Population& pop) const;
    int assessConflictSeverity(const Population& pop, const Economy& eco) const;
    
public:
    AIController();
    ~AIController(); // Destructor to clean up dynamically allocated memory
    
    // Main decision methods
    string makeTaxDecision(Economy& eco, Population& pop);
    string mobilizeArmy(Army& army, Population& pop, ResourceManager& res);
    string handleInternalConflict(Population& pop, Army& army, Economy& eco);
    
    // Methods to work with dynamic arrays
    void addDecision(int decisionCode);
    void updateUnitStrength(int unitType, float newStrength);
    void setResourcePriority(int resourceType, int priority);
};

template<typename T>
class SimpleArray {
private:
    T data[5];
    int size;
public:
    SimpleArray() : size(0) {}
    void add(const T& value) {
        if (size < 5) data[size++] = value;
    }
    T& operator[](int idx) { return data[idx]; }
    int getSize() const { return size; }
};

// ================== History Tracker ==================

// Structure to hold a single snapshot of game state
struct GameStateSnapshot {
    int turn;               // Game turn when snapshot was taken
    int population;         // Total population
    int treasury;          // Treasury amount
    int soldiers;          // Army size
    int morale;            // Army morale
    int food;              // Food resource
    int wood;              // Wood resource
    int stone;             // Stone resource
    int iron;              // Iron resource
    string eventDescription; // Description of major event in this turn
};

class HistoryTracker {
private:
    GameStateSnapshot* snapshots;  // Dynamic array of game state snapshots
    int capacity;                  // Current capacity of the snapshots array
    int size;                      // Current number of snapshots
    int currentTurn;              // Current game turn
    
    // Helper method to resize the snapshots array when needed
    void resizeSnapshotsArray();
    
public:
    HistoryTracker();
    ~HistoryTracker();
    
    // Take a snapshot of the current game state
    void takeSnapshot(const Population& pop, const Economy& eco, 
                     const Army& army, const ResourceManager& res,
                     const string& eventDescription = "");
    
    // Increment the turn counter
    void nextTurn();
    
    // Display the history as a progression report
    void displayProgressionReport() const;
    
    // Get the current turn
    int getCurrentTurn() const;
};

