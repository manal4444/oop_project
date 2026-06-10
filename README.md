# 🏰 Stronghold — Kingdom Management Simulator

A **C++ console-based kingdom management game** built as a semester project for Object-Oriented Programming (OOP) at FAST-NUCES.

## 📌 About
Stronghold is a turn-based strategy simulation where the player manages a kingdom — controlling population, army, economy, resources, and random events. An AI controller manages a rival non-player kingdom.

## 🚀 Features
- **Population System** — Simulate population growth, happiness, and social classes (peasants, merchants, nobles)
- **Army Management** — Recruit soldiers, train troops, manage morale and food supply
- **Economy & Taxation** — Manage treasury, taxation policies, and economic decisions
- **Resource Manager** — Handle kingdom resources and shortages
- **Bank System** — Financial operations and banking management
- **Event Manager** — Random events that affect the kingdom dynamically
- **AI Controller** — Non-player kingdom controlled by an automated AI system
- **Game Save/Load** — Full game state persistence using file I/O (.txt files)
- **History Tracker** — Records and reports complete kingdom history across turns

## 🛠️ Tech Stack
- **Language:** C++
- **Concepts Used:** OOP (Inheritance, Polymorphism, Encapsulation, Abstraction)
- **File I/O:** CSV-style .txt file persistence
- **Design:** Multi-file architecture with header files and separate .cpp modules

## 🗂️ Project Structure
```
├── main.cpp              # Entry point and game loop
├── Stronghold.h          # All class declarations
├── army.cpp              # Army system implementation
├── bank.cpp              # Banking system
├── economicsystem.cpp    # Economy and taxation
├── eventmanager.cpp      # Random event handling
├── historytracker.cpp    # Kingdom history recording
├── leader.cpp            # Leader/King class (Polymorphism)
├── population.cpp        # Population simulation
├── resourcemanager.cpp   # Resource management
├── aicontroller.cpp      # AI-controlled kingdom
├── GameSaver.cpp         # Unified save/load system
└── GameSaver.h           # GameSaver class declaration
```

## ▶️ How to Run
```bash
g++ -o stronghold main.cpp army.cpp bank.cpp economicsystem.cpp eventmanager.cpp historytracker.cpp leader.cpp population.cpp resourcemanager.cpp aicontroller.cpp GameSaver.cpp
./stronghold
```

## 👩‍💻 Author
**Manal Shahnawaz** — BS Software Engineering, FAST-NUCES 
📧 manalshahnawaz1@gmail.com  
🔗 [LinkedIn](https://linkedin.com/in/manal-shahnawaz-)
