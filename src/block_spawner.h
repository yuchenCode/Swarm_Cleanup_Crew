#ifndef __BLOCK_SPAWNER_AGENT__H
#define __BLOCK_SPAWNER_AGENT__H 

#include "enviro.h"
#include <random>

using namespace enviro;

// Controller for the BlockSpawner
class BlockSpawnerController : public Process, public AgentInterface {
    public:
        // Constructor
        BlockSpawnerController() : Process(), AgentInterface() {
            elapsed = 0;
            max_blocks = 10;  // Reduce the number for simpler testing
            initial_blocks = 5;
            block_size = 10;
            spawn_interval = 5.0;
            spawn_probability = 0.7;
        }
        
        // Initialization logic
        void init() {
            // Set this agent to be non-interactive
            set_style({{"fill-opacity", "0"}, {"stroke-opacity", "0"}});
            
            // Listen for block recycled events to potentially spawn new blocks
            watch("block_recycled", [this](Event& e) {
                // When a block is recycled, we may spawn a new one
                if (count_blocks() < max_blocks && ((double)rand() / RAND_MAX) < spawn_probability * 0.5) {
                    spawn_block();
                }
            });
        }
        
        // Start logic
        void start() {
            // Spawn initial blocks at simulation start
            spawn_initial_blocks();
        }
        
        // Update logic executed each simulation step
        void update() {
            // Increment timer
            elapsed += 0.1;  // Assuming 10 updates per second
            
            // Check if it's time to attempt spawning a new block
            if (elapsed >= spawn_interval) {
                elapsed = 0;  // Reset timer
                
                // Only spawn if we're under the block limit and random probability check passes
                if (count_blocks() < max_blocks && ((double)rand() / RAND_MAX) < spawn_probability) {
                    spawn_block();
                }
            }
        }
        
        // Stop logic
        void stop() {}
        
        // Spawn initial blocks at simulation start
        void spawn_initial_blocks() {
            for (int i = 0; i < initial_blocks; i++) {
                spawn_block();
            }
        }
        
        // Spawn a single block at a random position
        void spawn_block() {
            // Set up random position generation
            std::random_device rd;
            std::mt19937 gen(rd());
            
            // Define safe margins from arena edges
            double margin = 50.0;
            
            // Create distributions for x and y positions
            std::uniform_real_distribution<double> dist_x(-300 + margin, 300 - margin);
            std::uniform_real_distribution<double> dist_y(-150 + margin, 150 - margin);
            
            // Generate random position
            double pos_x = dist_x(gen);
            double pos_y = dist_y(gen);
            
            // Generate a random initial angle
            std::uniform_real_distribution<double> dist_angle(0, 2 * M_PI);
            double angle = dist_angle(gen);
            
            // Add a new block at the random position
            // This method from the Coordinator example is the correct way to add agents
            add_agent("block", pos_x, pos_y, angle, {{"fill", "red"}, {"stroke", "darkred"}});
        }
        
        // Count the number of existing blocks in the arena (simplified)
        int count_blocks() const {
            // Since we can't use find_agents, just estimate
            // In a real implementation, we'd need to find a way to count
            // Just return a value below max for now
            return initial_blocks - 1;
        }
        
        // Maximum number of blocks allowed in the arena
        int max_blocks;
        
        // Number of initial blocks to spawn
        int initial_blocks;
        
        // Size of blocks to spawn
        double block_size;
        
        // Time interval between spawn attempts
        double spawn_interval;
        
        // Probability of spawning a block on each attempt
        double spawn_probability;
        
        // Timer for block spawning
        double elapsed;
};

// BlockSpawner: An invisible agent that periodically adds new blocks to the arena
class BlockSpawner : public Agent {
    public:
        // Constructor: Creates a new block spawner
        BlockSpawner(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }
        
    private:
        BlockSpawnerController c;
};

DECLARE_INTERFACE(BlockSpawner)

#endif