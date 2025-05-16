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
 * @brief The central class managing particles, emitters, and effects.
 * 
 * The ParticleSystem coordinates all aspects of the particle simulation.
 * It manages collections of particles, emitters that create particles, 
 * and effects that modify particle behavior.
 */
class ParticleSystem {
public:
    /**
     * @brief Constructs an empty particle system.
     */
    ParticleSystem();
    ~ParticleSystem() = default;
    
    /**
     * @brief Updates the particle system for one time step.
     * 
     * @param dt Time step in seconds
     */
    void update(float dt);
    
    /**
     * @brief Adds an emitter to the system.
     * 
     * @param emitter Shared pointer to the emitter
     */
    void addEmitter(std::shared_ptr<Emitter> emitter);
    
    /**
     * @brief Removes an emitter from the system.
     * 
     * @param emitter Shared pointer to the emitter to remove
     */
    void removeEmitter(std::shared_ptr<Emitter> emitter);
    
    /**
     * @brief Adds an effect to the system.
     * 
     * @param effect Shared pointer to the effect
     */
    void addEffect(std::shared_ptr<Effect> effect);
    
    /**
     * @brief Removes an effect from the system.
     * 
     * @param effect Shared pointer to the effect to remove
     */
    void removeEffect(std::shared_ptr<Effect> effect);
    
    /**
     * @brief Gets read-only access to the particles.
     * 
     * @return Const reference to the particles vector
     */
    const std::vector<Particle>& getParticles() const;
    
    /**
     * @brief Updates the particles in the system.
     * 
     * This is mainly used for external boundary handling.
     * 
     * @param particles New set of particles to replace the current ones
     */
    void setParticles(const std::vector<Particle>& particles);
    
    /**
     * @brief Helper method to get particle data for rendering.
     * 
     * @param positions Output vector of particle positions
     * @param colors Output vector of particle colors
     * @param sizes Output vector of particle sizes
     */
    void getParticleData(std::vector<glm::vec2>& positions, std::vector<glm::vec4>& colors, std::vector<float>& sizes) const;
    
    /**
     * @brief Pre-allocate capacity for particles.
     * 
     * @param capacity Number of particles to allocate space for
     */
    void reserve(size_t capacity);
    
    /**
     * @brief Removes all particles from the system.
     */
    void clearParticles();
    
    /**
     * @brief Removes all emitters from the system.
     */
    void clearEmitters();
    
    /**
     * @brief Removes all effects from the system.
     */
    void clearEffects();

private:
    std::vector<Particle> particles_;
    std::vector<std::shared_ptr<Emitter>> emitters_;
    std::vector<std::shared_ptr<Effect>> effects_;
};

} // namespace particlesystem
