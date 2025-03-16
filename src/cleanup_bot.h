#ifndef __CLEANUP_BOT_AGENT__H
#define __CLEANUP_BOT_AGENT__H 

#include "enviro.h"
#include <string>
#include <vector>

using namespace enviro;

// Controller for CleanupBot
class CleanupBotController : public Process, public AgentInterface {
    public:
        CleanupBotController() : Process(), AgentInterface() {
            counter = 0;
            speed = 30;
        }

        // Initialization logic
        void init() {
            // Nothing special to do at init
        }
        
        // Start logic
        void start() {}
        
        // Update logic executed in each simulation step
        void update() {
            // Just move around in a pattern
            omni_apply_force(
                speed * cos(counter * 0.01),
                speed * sin(counter * 0.01)
            );
            counter++;
            
            // Apply damping to smooth movement
            omni_damp_movement();
        }
        
        // Stop logic
        void stop() {}
        
        // Process counter for movement
        int counter;
        
        // Speed of the robot
        double speed;
};

// CleanupBot: A robot that searches for blocks and brings them to recycling zones
class CleanupBot : public Agent {
    public:
        // Constructor: Creates a new cleanup bot
        CleanupBot(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }
        
    private:
        CleanupBotController c;
};

DECLARE_INTERFACE(CleanupBot)

#endif