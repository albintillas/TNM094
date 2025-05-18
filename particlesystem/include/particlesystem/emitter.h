#pragma once

#include <particlesystem/particle.h>
#include <vector>
#include <glm/vec2.hpp>

namespace particlesystem {

/**
 * Base class for all particle emitters.
 * Responsible for creating particles and adding them to the system.
 * Derived classes implement specific emission patterns.
 */
class Emitter {
public:
    /**
     * Creates an emitter at the specified position.
     */
    Emitter(const glm::vec2& position);
    virtual ~Emitter() = default;

    /**
     * Gets the current position of the emitter.
     */
    const glm::vec2& getPosition() const;
    
    /**
     * Sets a new position for the emitter.
     */
    void setPosition(const glm::vec2& position);

    /**
     * Sets the emission rate (particles per second).
     * Derived classes may override this for custom behavior.
     */
    virtual void setRate(float rate);
    
    /**
     * Gets the current emission rate.
     */
    float getRate() const;

    /**
     * Emits particles according to the emitter's pattern.
     * Must be implemented by derived classes.
     */
    virtual void emit(std::vector<Particle>& particles, float dt) = 0;

protected:
    glm::vec2 position_;     // Position of the emitter
    float rate_;             // Emission rate in particles per second
    float accumulator_;      // Accumulates time to control emission rate
};

} // namespace particlesystem 