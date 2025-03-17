#ifndef __RECYCLING_ZONE_AGENT__H
#define __RECYCLING_ZONE_AGENT__H 

#include "enviro.h"

using namespace enviro;

// Controller for the recycling zone
class RecyclingZoneController : public Process, public AgentInterface {
    public:
        // Constructor
        RecyclingZoneController() : Process(), AgentInterface() {
            width = 100;
            height = 100;
            color = "green";
        }
        
        // Initialization logic
        void init() {
            // Set visual style based on the color assigned in the constructor
            set_style({{"fill", color}, {"fill-opacity", "0.3"}, {"stroke", "black"}, {"stroke-width", "2"}});
        }
        
        // Start logic
        void start() {}
        
        // Update logic executed each simulation step
        void update() {}
        
        // Stop logic
        void stop() {}
        
        // Width of the zone
        double width;
        
        // Height of the zone
        double height;
        
        // Color for visual identification
        std::string color;
};

// RecyclingZone: A stationary area where blocks can be recycled
class RecyclingZone : public Agent {
    public:
        // Constructor: Creates a new recycling zone
        RecyclingZone(json spec, World& world) : Agent(spec, world) {
            // Assign a color for visual identification
            static const std::string colors[] = {"green", "blue", "purple"};
            static int color_index = 0;
            c.color = colors[color_index % 3];
            color_index++;
            
            add_process(c);
        }
        
    private:
        RecyclingZoneController c;
};

DECLARE_INTERFACE(RecyclingZone)

#endif