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
            carrier_id = -1;
            recycling = false;
        }
        
        // Initialization logic
        void init() {
            // Listen for carry events
            watch("carry_block", [this](Event& e) {
                int potential_carrier = e.value()["carrier_id"];
                double carrier_x = e.value()["x"];
                double carrier_y = e.value()["y"];
                
                // Check if this event is intended for this block
                if (!carried) {
                    double dx = carrier_x - x();
                    double dy = carrier_y - y();
                    double dist = sqrt(dx*dx + dy*dy);
                    
                    if (dist < 30) {  // If the potential carrier is close enough
                        carried = true;
                        carrier_id = potential_carrier;
                        
                        // Apply physics changes to make it act like it's being carried
                        apply_force(0, 0);  // Stop any motion
                    }
                }
            });
            
            // Listen for release events
            watch("release_block", [this](Event& e) {
                int releasing_carrier = e.value()["carrier_id"];
                
                // Only release if this is our carrier
                if (carried && releasing_carrier == carrier_id) {
                    carried = false;
                    carrier_id = -1;
                }
            });
            
            // Listen for recycling zone events
            watch("block_recycled", [this](Event& e) {
                int block_id = e.value()["block_id"];
                
                // Check if this is the block being recycled
                if (block_id == id()) {
                    recycling = true;
                }
            });
        }
        
        // Start logic
        void start() {}
        
        // Update logic executed each simulation step
        void update() {
            // Check if this block is marked for recycling
            if (recycling) {
                // We can't directly remove the agent, so we'll move it far away
                teleport(10000, 10000, 0);
                return;
            }
            
            // Watch for carrier position events
            watch("carrier_position", [this](Event& e) {
                int e_id = e.value()["id"];
                
                if (carried && e_id == carrier_id) {
                    double pos_x = e.value()["x"];
                    double pos_y = e.value()["y"];
                    double angle = e.value()["theta"];
                    
                    // Calculate offset position (slightly behind the carrier)
                    double offset_distance = 25.0;
                    double offset_angle = angle + M_PI;  // Behind the carrier
                    
                    double target_x = pos_x + offset_distance * cos(offset_angle);
                    double target_y = pos_y + offset_distance * sin(offset_angle);
                    
                    // Move the block to follow the carrier
                    teleport(target_x, target_y, angle);
                }
            });
        }
        
        // Stop logic
        void stop() {}
        
        // Check if this block is being carried
        bool is_carried() const { return carried; }

    private:
        // Flag indicating if this block is being carried
        bool carried;
        
        // ID of the agent carrying this block (if any)
        int carrier_id;
        
        // Flag indicating if this block should be recycled
        bool recycling;
};

// Block: A simple object that can be picked up by cleanup robots
class Block : public Agent {
    public:
        // Constructor: Creates a new block
        Block(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }
        
        // Initialize to prevent rotation
        void init() {
            // Apply a high damping instead of prevent_rotation
            apply_force(0, 0);
        }
        
    private:
        BlockController c;
};

DECLARE_INTERFACE(Block)

#endif