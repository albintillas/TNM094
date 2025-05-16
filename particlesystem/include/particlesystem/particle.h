#pragma once

#include <glm/vec2.hpp>

namespace particlesystem {

/**
 * @brief The fundamental unit of the particle system.
 * 
 * A Particle represents a single point element in the simulation with
 * position, velocity, and other physical properties. Particles are created
 * by Emitters and can be affected by Effects.
 */
class Particle {
public:
    /**
     * @brief Constructs a default particle.
     * 
     * Creates a particle at the origin with zero velocity and zero force.
     * The particle is initially not alive.
     */
    Particle();
    ~Particle() = default;

    // Core properties
    glm::vec2 position;  ///< Current position of the particle
    glm::vec2 velocity;  ///< Current velocity of the particle
    glm::vec2 force;     ///< Current accumulated force acting on the particle
    float lifetime;      ///< Remaining lifetime in seconds
    bool alive;          ///< Whether the particle is active in the simulation

    /**
     * @brief Updates the particle state for a time step.
     * 
     * Applies physics (velocity and forces) and decreases lifetime.
     * If lifetime reaches zero, the particle is marked as not alive.
     * 
     * @param dt Time step in seconds
     */
    void update(float dt);
    
    /**
     * @brief Resets accumulated force to zero.
     * 
     * Called at the beginning of each simulation step to prepare
     * for force accumulation from effects.
     */
    void resetForce();
};

} // namespace particlesystem 