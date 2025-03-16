#ifndef __BLOCK_SPAWNER_AGENT__H
#define __BLOCK_SPAWNER_AGENT__H 

#include "enviro.h"
#include <random>

using namespace enviro;

// Forward declaration for the block spawner
class BlockSpawner;

// Controller for the BlockSpawner
class BlockSpawnerController : public Process, public AgentInterface {
    public:
        // Constructor
        BlockSpawnerController(BlockSpawner* agent) : Process(), AgentInterface(), spawner(agent) {
            elapsed = 0;
            spawn_interval = 5.0;
        }
        
        // Initialization logic
        void init() {
            // Spawn initial blocks - we'll do this in start() to ensure the Agent is fully initialized
        }
        
        // Start logic
        void start() {
            // Spawn initial blocks - we won't actually spawn blocks in this simplest version
            // Just to get it to compile
        }
        
        // Update logic executed each simulation step
        void update() {
            // In the simplest version, we won't do anything
            // Just to get it to compile
        }
        
        // Stop logic
        void stop() {}
        
        // Timer for spawning
        double elapsed;
        double spawn_interval;
        
        // Reference to the spawner agent
        BlockSpawner* spawner;
};

// BlockSpawner: An invisible agent that periodically adds new blocks to the arena
class BlockSpawner : public Agent {
    public:
        // Constructor: Creates a new block spawner
        BlockSpawner(json spec, World& world) : Agent(spec, world), c(this) {
            add_process(c);
        }
        
    private:
        BlockSpawnerController c;
};

DECLARE_INTERFACE(BlockSpawner)

#endif