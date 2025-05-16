#pragma once

#include <particlesystem/particle.h>
#include <vector>
#include <glm/vec2.hpp>

namespace particlesystem {

/**
 * @brief Base class for all particle emitters.
 * 
 * An emitter is responsible for creating particles and adding them
 * to the particle system. This abstract base class provides common 
 * functionality for all emitter types, including position and emission rate.
 * Derived classes implement specific emission patterns.
 */
class Emitter {
public:
    /**
     * @brief Constructs an emitter at the specified position.
     * 
     * @param position The position of the emitter in 2D space
     */
    Emitter(const glm::vec2& position);
    virtual ~Emitter() = default;

    /**
     * @brief Gets the current position of the emitter.
     * 
     * @return The position as a 2D vector
     */
    const glm::vec2& getPosition() const;
    
    /**
     * @brief Sets a new position for the emitter.
     * 
     * @param position The new position as a 2D vector
     */
    void setPosition(const glm::vec2& position);

    /**
     * @brief Sets the emission rate (particles per second).
     * 
     * Derived classes may override this to implement custom rate behavior.
     * 
     * @param rate Number of particles to emit per second
     */
    virtual void setRate(float rate);
    
    /**
     * @brief Gets the current emission rate.
     * 
     * @return The emission rate in particles per second
     */
    float getRate() const;

    /**
     * @brief Emits particles according to the emitter's pattern.
     * 
     * This pure virtual method must be implemented by derived classes
     * to define the specific emission behavior.
     * 
     * @param particles Vector to add new particles to
     * @param dt Time step in seconds
     */
    virtual void emit(std::vector<Particle>& particles, float dt) = 0;

protected:
    glm::vec2 position_;     ///< Position of the emitter
    float rate_;             ///< Emission rate in particles per second
    float accumulator_;      ///< Accumulates time to control emission rate
};

} // namespace particlesystem 