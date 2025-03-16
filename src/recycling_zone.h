#ifndef __RECYCLING_ZONE_AGENT__H
#define __RECYCLING_ZONE_AGENT__H 

#include "enviro.h"

using namespace enviro;

// Controller for the recycling zone
class RecyclingZoneController : public Process, public AgentInterface {
    public:
        // Constructor
        RecyclingZoneController() : Process(), AgentInterface() {}
        
        // Initialization logic
        void init() {}
        
        // Start logic
        void start() {}
        
        // Update logic executed each simulation step
        void update() {
            // Nothing special to do here
        }
        
        // Stop logic
        void stop() {}
};

// RecyclingZone: A stationary area where blocks can be recycled
class RecyclingZone : public Agent {
    public:
        // Constructor: Creates a new recycling zone
        RecyclingZone(json spec, World& world) : Agent(spec, world) {
            add_process(c);
        }
        
    private:
        RecyclingZoneController c;
};

DECLARE_INTERFACE(RecyclingZone)

#endif