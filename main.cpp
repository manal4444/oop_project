#include <iostream>
#include "Stronghold.h"  // Your header with all class declarations


using namespace std;

int main() {
    // Initialize core systems (basic object instantiations)
    Population populationSystem;
    Army armySystem;
    Economy economySystem;
    ResourceManager resourceSystem;
    Leader* currentLeader = new King();  // Polymorphic leader
    EventManager eventSystem;
    Bank bankSystem;
    GameSaver gameSaver;  // Initialize the GameSaver for unified saving/loading
    HistoryTracker historyTracker;  // Initialize the HistoryTracker for recording game history

    int choice;
    bool running = true;

    while (running) {
        cout << "\n================ STRONGHOLD MENU ================\n";
        cout << "1. View Kingdom Overview\n";
        cout << "2. Simulate Population Changes\n";
        cout << "3. Recruit and Train Army\n";
        cout << "4. Manage Economy (Taxation, Treasury)\n";
        cout << "5. Handle Resource Operations\n";
        cout << "6. Trigger Random Event\n";
        cout << "7. Save Game to File\n";
        cout << "8. Load Game from File\n";
        cout << "9. View Kingdom History Report\n";
        cout << "10. Advance to Next Turn\n";
        cout << "11. Let AI Control Non-Player Kingdom\n";
        cout << "12. Exit\n";
        cout << "=================================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Input validation
        if (cin.fail() || choice < 1 || choice > 12) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number between 1 and 12.\n";
            continue;
        }

        // Action handling
        switch (choice) {
            case 1:
                populationSystem.showStats();
                armySystem.showStats();
                economySystem.showStats();
                resourceSystem.showStats();
                bankSystem.showStats();
                break;

            case 2:
                populationSystem.simulate();
                // Take a snapshot after population changes
                historyTracker.takeSnapshot(populationSystem, economySystem, armySystem, resourceSystem, "Population simulation");
                break;

            case 3:
                armySystem.recruitAndTrain(populationSystem);
                // Take a snapshot after army recruitment
                historyTracker.takeSnapshot(populationSystem, economySystem, armySystem, resourceSystem, "Army recruitment");
                break;

            case 4:
                economySystem.taxPopulation(populationSystem);
                bankSystem.auditTreasury(economySystem);
                // Take a snapshot after economic changes
                historyTracker.takeSnapshot(populationSystem, economySystem, armySystem, resourceSystem, "Tax collection");
                break;

            case 5:
                resourceSystem.manage();
                break;

            case 6:
                eventSystem.trigger(populationSystem, armySystem, economySystem, resourceSystem);
                break;

            case 7:
                // Use GameSaver to save all game state to a single file
                if (gameSaver.saveGame(populationSystem, armySystem, economySystem, resourceSystem, bankSystem)) {
                    cout << "Game saved successfully to game_save.txt\n";
                } else {
                    cout << "Failed to save game state\n";
                }
                break;

            case 8:
                // Use GameSaver to load all game state from a single file
                if (gameSaver.loadGame(populationSystem, armySystem, economySystem, resourceSystem, bankSystem)) {
                    cout << "Game loaded successfully from game_save.txt\n";
                } else {
                    cout << "Failed to load game state\n";
                }
                break;

            case 9:
                // Display the kingdom history report
                historyTracker.displayProgressionReport();
                break;
            case 10: {
                cout << "\n=========== Kingdom State Before AI Actions ===========\n";
                populationSystem.showStats();
                armySystem.showStats();
                economySystem.showStats();
                resourceSystem.showStats();

                cout << "\n============= AI Decision Making Process =============\n";
                AIController ai;
                
                // Show AI tax management decision and effects
                string taxReport = ai.makeTaxDecision(economySystem, populationSystem);
                cout << taxReport;
                
                // Show AI army management decision and effects
                string armyReport = ai.mobilizeArmy(armySystem, populationSystem, resourceSystem);
                cout << armyReport;
                
                // Show AI conflict management decision and effects
                string conflictReport = ai.handleInternalConflict(populationSystem, armySystem, economySystem);
                cout << conflictReport;

                cout << "\n=========== Kingdom State After AI Actions ===========\n";
                populationSystem.showStats();
                armySystem.showStats();
                economySystem.showStats();
                resourceSystem.showStats();
                
                // Take a snapshot after AI actions
                historyTracker.takeSnapshot(populationSystem, economySystem, armySystem, resourceSystem, "AI turn actions");
                // Advance to next turn
                historyTracker.nextTurn();
                break;
            }
            
            case 11: {
                cout << "\n============= AI Kingdom Control =============\n";
                cout << "AI is now controlling a non-player kingdom...";
                
                // Create a separate AI-controlled kingdom
                Population aiPopulation;
                Army aiArmy;
                Economy aiEconomy;
                ResourceManager aiResources;
                AIController aiController;
                
                // Show initial state of AI kingdom
                cout << "\n=========== AI Kingdom Initial State ===========\n";
                aiPopulation.showStats();
                aiArmy.showStats();
                aiEconomy.showStats();
                aiResources.showStats();
                
                // AI makes decisions for its kingdom
                cout << "\n============= AI Kingdom Actions =============\n";
                
                // AI manages taxes
                string taxReport = aiController.makeTaxDecision(aiEconomy, aiPopulation);
                cout << taxReport;
                
                // AI manages army
                string armyReport = aiController.mobilizeArmy(aiArmy, aiPopulation, aiResources);
                cout << armyReport;
                
                // AI handles internal conflicts
                string conflictReport = aiController.handleInternalConflict(aiPopulation, aiArmy, aiEconomy);
                cout << conflictReport;
                
                // Show final state of AI kingdom
                cout << "\n=========== AI Kingdom Final State ===========\n";
                aiPopulation.showStats();
                aiArmy.showStats();
                aiEconomy.showStats();
                aiResources.showStats();
                
                cout << "\nAI kingdom simulation complete.\n";
                break;
            }
            
            case 12:
                // Exit the game
                running = false;
                break;
        }
    }

    delete currentLeader;  // clean-up if not using smart pointers
    cout << "\nGame exited successfully. Long live the kingdom!\n";
    return 0;
}
