#ifndef __CLEANUP_BOT_AGENT__H
#define __CLEANUP_BOT_AGENT__H 

#include "enviro.h"
#include <string>
#include <vector>

using namespace enviro;

// Enumeration for robot states
enum class BotState {
    SEARCHING,  // Looking for blocks
    CARRYING,   // Carrying a block
    DEPOSITING  // Moving to recycling zone
};

// Controller for CleanupBot
class CleanupBotController : public Process, public AgentInterface {
    public:
        CleanupBotController() : Process(), AgentInterface() {
            state = BotState::SEARCHING;
            carried_block_id = -1;
            counter = 0;
            state_time = 0;
            speed = 0.5;
            sensor_range = 120;
            repulsion_range = 40;
            repulsion_force = 300;
        }

        // Initialization logic called when the robot is added to the world
        void init() {
            // Register collision handler to interact with blocks
            notice_collisions_with("block", [this](Event& e) {
                // Get the Agent ID from the event
                int agent_id = e.value()["id"];
                
                if (state == BotState::SEARCHING) {
                    // We've collided with a block and can pick it up
                    this->carry_block(agent_id);
                    state = BotState::CARRYING;
                    state_time = 0;
                }
            });
            
            // Register collision handler to detect recycling zones
            notice_collisions_with("recycling_zone", [this](Event& e) {
                if (state == BotState::CARRYING && carried_block_id >= 0) {
                    // Emit block_recycled event
                    json j = { {"block_id", carried_block_id} };
                    emit(Event("block_recycled", j));
                    
                    // Release the block
                    release_block();
                    state = BotState::SEARCHING;
                    state_time = 0;
                }
            });
        }
        
        // Start logic
        void start() {}
        
        // Update logic executed in each simulation step
        void update() {
            // Update state time
            state_time += 0.1;
            
            // Apply forces to avoid obstacles and other robots
            apply_avoidance_forces();
            
            // Update state based on current situation
            update_state();
            
            // State-specific behaviors
            if (state == BotState::SEARCHING) {
                // Random movement for now
                omni_apply_force(
                    speed * cos(counter * 0.01),
                    speed * sin(counter * 0.01)
                );
                counter++;
            } 
            else if (state == BotState::CARRYING && carried_block_id >= 0) {
                // Move toward one of the recycling zones (simple approach)
                omni_move_toward(0, -150, speed * 0.7);
                
                // Emit position event for any carried blocks to follow
                json j = {
                    {"id", id()},
                    {"x", x()},
                    {"y", y()},
                    {"theta", angle()}
                };
                emit(Event("carrier_position", j));
            }
            
            // Apply damping to smooth movement
            omni_damp_movement();
        }
        
        // Stop logic
        void stop() {}
        
        // Pick up a block
        void carry_block(int block_id) {
            if (carried_block_id < 0) {
                carried_block_id = block_id;
                
                // Send an event to notify the block that it's being carried
                json j = {
                    {"carrier_id", id()},
                    {"x", x()},
                    {"y", y()}
                };
                emit(Event("carry_block", j));
            }
        }
        
        // Release the carried block
        void release_block() {
            if (carried_block_id >= 0) {
                // Send an event to notify the block that it's being released
                json j = {{"carrier_id", id()}};
                emit(Event("release_block", j));
                
                carried_block_id = -1;
            }
        }
        
        // Apply avoidance forces to prevent collisions with obstacles and other robots
        void apply_avoidance_forces() {
            // We'll simplify to just have some basic avoidance
            // Use range sensors if available, otherwise just apply some random forces
            double noise_level = 0.1;
            double fx = noise_level * (2 * (double)rand() / RAND_MAX - 1);
            double fy = noise_level * (2 * (double)rand() / RAND_MAX - 1);
            omni_apply_force(fx, fy);
        }
        
        // Update the bot's state based on current conditions
        void update_state() {
            // Check for state transitions
            if (state == BotState::SEARCHING) {
                // Nothing to do when searching, transition happens when a block is found
            } 
            else if (state == BotState::CARRYING) {
                // Check if we lost the block somehow
                if (carried_block_id < 0) {
                    state = BotState::SEARCHING;
                    state_time = 0;
                }
            }
        }
        
        // Speed of the robot
        double speed;
        
        // Range of sensors
        double sensor_range;
        
        // Repulsion range and force
        double repulsion_range;
        double repulsion_force;
        
    private:
        // Current state of the robot
        BotState state;
        
        // ID of block being carried (if any)
        int carried_block_id;
        
        // Process counter to prevent state switching too quickly
        int counter;
        
        // Target position to move towards
        double target_x, target_y;
        
        // Time spent in current state
        double state_time;
        
        // Number of sensors
        static const int NUM_SENSORS = 8;
        
        // PI constant
        static constexpr double PI = 3.14159265358979323846;
        
        // Threshold distance for movement
        static constexpr double THRESHOLD_DIST = 10.0;
};

// CleanupBot: A robot that searches for blocks and brings them to recycling zones
class CleanupBot : public Agent {
    public:
        // Constructor: Creates a new cleanup bot
        CleanupBot(json spec, World& world) : Agent(spec, world) {
            // We'll set configuration values with defaults
            c.speed = 0.5;
            c.sensor_range = 120;
            c.repulsion_range = 40;
            c.repulsion_force = 300;
            
            add_process(c);
        }
        
    private:
        CleanupBotController c;
};

DECLARE_INTERFACE(CleanupBot)

#endif