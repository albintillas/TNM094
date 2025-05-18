#pragma once

#include <particlesystem/particle.h>
#include <particlesystem/emitter.h>
#include <particlesystem/effect.h>
#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace particlesystem {

/**
 * Central class managing particles, emitters, and effects.
 * Coordinates all aspects of the particle simulation.
 */
class ParticleSystem {
public:
    /**
     * Creates an empty particle system.
     */
    ParticleSystem();
    ~ParticleSystem() = default;
    
    /**
     * Updates the particle system for one time step.
     * dt: Time step in seconds
     */
    void update(float dt);
    
    /**
     * Adds an emitter to the system.
     */
    void addEmitter(std::shared_ptr<Emitter> emitter);
    
    /**
     * Removes an emitter from the system.
     */
    void removeEmitter(std::shared_ptr<Emitter> emitter);
    
    /**
     * Adds an effect to the system.
     */
    void addEffect(std::shared_ptr<Effect> effect);
    
    /**
     * Removes an effect from the system.
     */
    void removeEffect(std::shared_ptr<Effect> effect);
    
    /**
     * Gets read-only access to the particles.
     * Returns const reference to the particles vector.
     */
    const std::vector<Particle>& getParticles() const;
    
    /**
     * Updates the particles in the system.
     * Mainly used for external boundary handling.
     */
    void setParticles(const std::vector<Particle>& particles);
    
    /**
     * Gets particle data for rendering.
     * Fills the provided vectors with position, color and size data.
     */
    void getParticleData(std::vector<glm::vec2>& positions, std::vector<glm::vec4>& colors, std::vector<float>& sizes) const;
    
    /**
     * Pre-allocate capacity for particles.
     */
    void reserve(size_t capacity);
    
    /**
     * Removes all particles from the system.
     */
    void clearParticles();
    
    /**
     * Removes all emitters from the system.
     */
    void clearEmitters();
    
    /**
     * Removes all effects from the system.
     */
    void clearEffects();

private:
    std::vector<Particle> particles_;
    std::vector<std::shared_ptr<Emitter>> emitters_;
    std::vector<std::shared_ptr<Effect>> effects_;
};

} // namespace particlesystem
