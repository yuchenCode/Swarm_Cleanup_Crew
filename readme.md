# Swarm Cleanup Crew

## Overview

Swarm Cleanup Crew is a multi-agent simulation built on the Enviro framework. In this simulation, a team of small omni-directional robots work together to clean an arena by collecting scattered blocks and depositing them into designated recycling zones.

The simulation demonstrates principles of multi-agent systems, sensor-based navigation, and environmental interaction in a game-like context. Each robot autonomously searches for blocks, picks them up, and brings them to one of the recycling zones while avoiding obstacles and other robots.

## Key Features

1. **Cleanup Robots**: Small circular robots equipped with sensors to detect blocks and obstacles.
2. **Recycling Zones**: Static areas where blocks can be deposited for recycling.
3. **Dynamic Block Spawning**: New blocks periodically appear in the arena to keep the robots busy.
4. **Collaborative Behavior**: Robots naturally distribute themselves to efficiently clean the arena.
5. **Score Tracking**: The system counts how many blocks have been successfully recycled.

## Challenges and Solutions

### Challenge 1: Robot Clustering
**Problem**: Robots might cluster around a single recycling zone, leaving others unused.

**Solution**: I implemented position-based decision-making for the robots. When multiple robots are near the same recycling zone, they naturally spread out due to their collision avoidance behavior, leading to a more efficient distribution of work.

### Challenge 2: Block Detection and Handling
**Problem**: Efficiently detecting and picking up blocks without getting stuck or interfering with other robots.

**Solution**: Used proximity-based detection with simple state machines to control the robots' behavior. Each robot transitions between searching, carrying, and depositing states based on its current situation.

### Challenge 3: Dynamic Block Spawning
**Problem**: Managing dynamic block spawning without overwhelming the system.

**Solution**: Implemented controlled spawning with rate limiting. New blocks appear at a steady rate, and the system maintains a reasonable maximum number of blocks in the arena at any time.

### Challenge 4: Obstacle Avoidance
**Problem**: Robots need to navigate around walls and other obstacles efficiently.

**Solution**: Utilized Enviro's built-in sensor capabilities to implement reactive obstacle avoidance, allowing robots to smoothly navigate around barriers while pursuing their goals.

## Installation and Setup

### Prerequisites
- Docker installed on your system
- Git for cloning the repository

### Setup Steps

1. Clone the repository:
   ```
   git clone https://github.com/yuchenCode/Swarm_Cleanup_Crew.git
   cd Swarm_Cleanup_Crew
   ```

2. Run the Docker container with the Enviro environment:
   ```
   docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:v1.6 bash
   ```

3. Inside the Docker container, build the project:
   ```
   make
   ```

4. Start the simulation:
   ```
   enviro
   ```

5. Access the simulation in your web browser:
   ```
   http://localhost:8765
   ```

## Usage

When you launch the simulation, you'll see:
- Blue circular robots moving around the arena
- Red square blocks scattered throughout
- Colored rectangular zones at the bottom of the screen (recycling zones)
- Grey walls forming the boundaries and obstacles

The robots will autonomously:
1. Search for blocks
2. Pick them up when found
3. Carry blocks to recycling zones
4. Deposit blocks at the zones

New blocks will periodically appear to replace recycled ones. The score is tracked and displayed in the simulation.

## Project Structure

- `src/`: Source code for all agent types
  - `cleanup_bot.h`: Robot agents that search for and transport blocks
  - `block.h`: Collectable block objects
  - `recycling_zone.h`: Zones where blocks are deposited
  - `block_spawner.h`: Generates new blocks over time
  - `score_manager.h`: Keeps track of successfully recycled blocks
- `defs/`: JSON definitions for each agent type
- `config.json`: Main configuration file for the simulation
- `Makefile`: Build instructions

## Acknowledgments

- This project uses the [Enviro](https://github.com/klavinslab/enviro) framework developed by Eric Klavins
- Inspired by swarm robotics research and collective cleaning behaviors
- Built as a final project for EE/CSE P 478 at the University of Washington

## License

This project is licensed under the MIT License - see the LICENSE file for details.