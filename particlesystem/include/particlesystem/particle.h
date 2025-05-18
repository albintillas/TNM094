#pragma once

#include <glm/vec2.hpp>

namespace particlesystem {

/**
 * The fundamental unit of the particle system.
 * Represents a single point element with position, velocity, and other properties.
 */
class Particle {
public:
    /**
     * Creates a default particle at origin with zero velocity.
     * The particle is initially not alive.
     */
    Particle();
    ~Particle() = default;

    // Core properties
    glm::vec2 position;  // Current position of the particle
    glm::vec2 velocity;  // Current velocity of the particle
    glm::vec2 force;     // Current accumulated force acting on the particle
    float lifetime;      // Remaining lifetime in seconds
    bool alive;          // Whether the particle is active in the simulation

    /**
     * Updates the particle state for a time step.
     * Applies physics and decreases lifetime.
     */
    void update(float dt);
    
    /**
     * Resets accumulated force to zero.
     * Called at the beginning of each simulation step.
     */
    void resetForce();
};

} // namespace particlesystem 