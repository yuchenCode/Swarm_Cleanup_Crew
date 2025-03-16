#ifndef __BLOCK_AGENT__H
#define __BLOCK_AGENT__H 

#include "enviro.h"

using namespace enviro;

// Block Controller: Manages block behavior
class BlockController : public Process, public AgentInterface {
    public:
        // Constructor
        BlockController() : Process(), AgentInterface() {
            carried = false;
        }
        
        // Initialization logic
        void init() {
            // Nothing special to do here
        }
        
        // Start logic
        void start() {}
        
        // Update logic executed each simulation step
        void update() {
            // Do nothing special, the Block will just stay where it is
            // Robots will detect it based on position
            if (carried) {
                // If carried, move out of the way when recycled
                teleport(10000, 10000, 0);
            }
        }
        
        // Stop logic
        void stop() {}
        
        // Flag indicating if this block is being carried
        bool carried = false;
};

// Block: A simple object that can be picked up by cleanup robots
class Block : public Agent {
    public:
        // Constructor: Creates a new block
        Block(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }
        
    private:
        BlockController c;
};

DECLARE_INTERFACE(Block)

#endif