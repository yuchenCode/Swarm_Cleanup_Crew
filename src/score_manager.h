#ifndef __SCORE_MANAGER_AGENT__H
#define __SCORE_MANAGER_AGENT__H 

#include "enviro.h"

using namespace enviro;

// Controller for the ScoreManager
class ScoreManagerController : public Process, public AgentInterface {
    public:
        // Constructor
        ScoreManagerController() : Process(), AgentInterface() {}
        
        // Initialization logic
        void init() {}
        
        // Start logic
        void start() {}
        
        // Update logic executed each simulation step
        void update() {}
        
        // Stop logic
        void stop() {}
};

// ScoreManager: Tracks and displays the cleanup score
class ScoreManager : public Agent {
    public:
        // Constructor: Creates a new score manager
        ScoreManager(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }
        
    private:
        ScoreManagerController c;
};

DECLARE_INTERFACE(ScoreManager)

#endif