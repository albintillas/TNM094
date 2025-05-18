#pragma once

#include <particlesystem/particle.h>

namespace particlesystem {

/**
 * Base class for all particle effects.
 * Modifies the behavior of particles in the system.
 * Derived classes implement specific behaviors.
 */
class Effect {
public:
    /**
     * Creates an effect with default settings.
     * Strength of 1.0 and enabled by default.
     */
    Effect();
    virtual ~Effect() = default;
    
    /**
     * Sets the strength of the effect.
     * Controls how strongly the effect influences particles.
     */
    void setStrength(float strength);
    
    /**
     * Gets the current strength of the effect.
     */
    float getStrength() const;
    
    /**
     * Enables or disables the effect.
     * Disabled effects won't be applied during simulation.
     */
    void setEnabled(bool enabled);
    
    /**
     * Checks if the effect is currently enabled.
     */
    bool isEnabled() const;
    
    /**
     * Applies the effect to a particle.
     * Must be implemented by derived classes.
     */
    virtual void apply(Particle& particle) = 0;

protected:
    float strength_;  // Strength of the effect
    bool enabled_;    // Whether the effect is currently enabled
};

} // namespace particlesystem 