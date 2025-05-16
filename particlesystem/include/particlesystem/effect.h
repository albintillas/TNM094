#pragma once

#include <particlesystem/particle.h>

namespace particlesystem {

/**
 * @brief Base class for all particle effects.
 * 
 * An effect modifies the behavior of particles in the system.
 * This abstract base class provides common functionality for all effect types,
 * including strength and enable/disable state. Derived classes implement
 * specific modification behaviors.
 */
class Effect {
public:
    /**
     * @brief Constructs an effect with default settings.
     * 
     * Creates an effect with a strength of 1.0 that is enabled by default.
     */
    Effect();
    virtual ~Effect() = default;
    
    /**
     * @brief Sets the strength of the effect.
     * 
     * The strength parameter controls how strongly the effect influences particles.
     * 
     * @param strength The effect strength (typically in range [0,1])
     */
    void setStrength(float strength);
    
    /**
     * @brief Gets the current strength of the effect.
     * 
     * @return The effect strength value
     */
    float getStrength() const;
    
    /**
     * @brief Enables or disables the effect.
     * 
     * A disabled effect will not be applied to particles during simulation.
     * 
     * @param enabled True to enable the effect, false to disable
     */
    void setEnabled(bool enabled);
    
    /**
     * @brief Checks if the effect is currently enabled.
     * 
     * @return True if the effect is enabled, false otherwise
     */
    bool isEnabled() const;
    
    /**
     * @brief Applies the effect to a particle.
     * 
     * This pure virtual method must be implemented by derived classes
     * to define the specific effect behavior.
     * 
     * @param particle The particle to modify
     */
    virtual void apply(Particle& particle) = 0;

protected:
    float strength_;  ///< Strength of the effect
    bool enabled_;    ///< Whether the effect is currently enabled
};

} // namespace particlesystem 