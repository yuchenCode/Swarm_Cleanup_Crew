#ifndef __SCORE_MANAGER_AGENT__H
#define __SCORE_MANAGER_AGENT__H 

#include "enviro.h"

using namespace enviro;

// Controller for the ScoreManager
class ScoreManagerController : public Process, public AgentInterface {
    public:
        // Constructor
        ScoreManagerController() : Process(), AgentInterface() {
            score = 0;
            points_per_block = 10;
        }
        
        // Initialization logic
        void init() {
            // Set this agent to be non-interactive
            set_style({{"fill-opacity", "0"}, {"stroke-opacity", "0"}});
            
            // Listen for block recycled events to update score
            watch("block_recycled", [this](Event& e) {
                add_points(points_per_block);
            });
            
            // Listen for reset button clicks
            watch("button_click", [this](Event& e) {
                if (e.value()["name"] == "reset") {
                    reset_score();
                }
            });
            
            // Create initial score display
            update_display();
        }
        
        // Start logic
        void start() {}
        
        // Update logic executed each simulation step
        void update() {}
        
        // Stop logic
        void stop() {}
        
        // Add points to the score
        void add_points(int points) {
            score += points;
            update_display();
        }
        
        // Reset the score to zero
        void reset_score() {
            score = 0;
            update_display();
        }
        
        // Update the score display
        void update_display() {
            // Clear any existing label
            clear_label();
            
            // Add a new label with the current score
            label("Score: " + std::to_string(score), -10, -10);
        }
        
        // Current score
        int score;
        
        // Points earned per recycled block
        int points_per_block;
};

// ScoreManager: Tracks and displays the cleanup score
class ScoreManager : public Agent {
    public:
        // Constructor: Creates a new score manager
        ScoreManager(json spec, World& world) : Agent(spec, world) {
            // Use default values
            add_process(c);
        }
        
    private:
        ScoreManagerController c;
};

DECLARE_INTERFACE(ScoreManager)

#endif